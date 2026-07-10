//
// Created by Limo on 09/06/2026.
//

#include "util/RegistryHelper.hpp"

#include "util/Log.hpp"
#include "UObject.hpp"
#include "UObjectGlobals.hpp"

using namespace RC;
using namespace Unreal;

static UObject *assetRegistrySubsystem = nullptr;

void RegistryHelper::InitCache() {
    if (!assetRegistrySubsystem) {
        std::vector<UObject*> list{};
        UObjectGlobals::FindAllOf(L"UWEAssetRegistrySubsystem", list);

        for (const auto& obj : list) {
            if (!obj->GetFullName().contains(L"/Engine/Transient.GameEngine"))
                continue;
            assetRegistrySubsystem = obj;
            break;
        }
    }
}

uintptr_t RegistryHelper::StaticConstructTemplateInternal(UObject *base, const std::string &name, const std::optional<UObject*> overrideOuter) {
    FStaticConstructObjectParameters params(
        base->GetClassPrivate(),
        overrideOuter.value_or(base->GetOuterPrivate())
    );

    params.Name = FName(UtfN::StringToWString(name).c_str());
    params.SetFlags = static_cast<EObjectFlags>(
        RF_MarkAsRootSet |
        RF_Public |
        RF_Standalone |
        RF_Transactional |
        RF_WasLoaded |
        RF_LoadCompleted);
    return reinterpret_cast<uintptr_t>(UObjectGlobals::StaticConstructObject(params));
}

void RegistryHelper::AddToRegistry(UUWEPrimaryDataAssetBase *asset, const std::string &type) {
    InitCache();

    if (assetRegistrySubsystem == nullptr)
        return;

    for (const auto assets = assetRegistrySubsystem
        ->GetPropertyByName(L"CachedDataAssets")
        ->ContainerPtrToValuePtr<TMap<UClass*, FUWEPrimaryDataAssetBaseArray>>(assetRegistrySubsystem);
    auto& pair : *assets) {
        if (UtfN::WStringToString(pair.Key->GetName()) != type)
            continue;

        pair.Value.Assets.Add(asset);
        return;
    }
    Log::Warning("Failed to register data asset to registry: {}", type);
}
