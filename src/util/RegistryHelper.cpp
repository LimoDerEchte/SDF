//
// Created by Limo on 09/06/2026.
//

#include "util/RegistryHelper.hpp"

#include "util/Log.hpp"
#include "UObject.hpp"
#include "UObjectGlobals.hpp"

using namespace SDK;
using namespace RC;
using namespace Unreal;

static UUWEAssetRegistrySubsystem *assetRegistrySubsystem = nullptr;

void RegistryHelper::InitCache() {
    if (!assetRegistrySubsystem) {
        std::vector<Unreal::UObject*> list{};
        UObjectGlobals::FindAllOf(L"UWEAssetRegistrySubsystem", list);

        for (const auto& obj : list) {
            if (!obj->GetFullName().contains(L"/Engine/Transient.GameEngine"))
                continue;

            assetRegistrySubsystem = reinterpret_cast<UUWEAssetRegistrySubsystem*>(obj);
        }
    }
}

uintptr_t RegistryHelper::StaticConstructTemplateInternal(SDK::UObject *base, const std::string &name, const std::optional<SDK::UObject *> overrideOuter) {
    FStaticConstructObjectParameters params(
        reinterpret_cast<Unreal::UClass*>(base->Class),
        reinterpret_cast<Unreal::UObject*>(overrideOuter.value_or(base->Outer))
    );

    params.Name = Unreal::FName(UtfN::StringToWString(name).c_str());
    params.SetFlags = static_cast<Unreal::EObjectFlags>(
        SDK::EObjectFlags::MarkAsRootSet |
        SDK::EObjectFlags::Public |
        SDK::EObjectFlags::Standalone |
        SDK::EObjectFlags::Transactional |
        SDK::EObjectFlags::WasLoaded |
        SDK::EObjectFlags::LoadCompleted);
    return reinterpret_cast<uintptr_t>(UObjectGlobals::StaticConstructObject(params));
}

void RegistryHelper::AddToRegistry(UUWEPrimaryDataAssetBase *asset, const std::string &type) {
    InitCache();


    for (auto& pair : assetRegistrySubsystem->CachedDataAssets) {
        if (UtfN::WStringToString(reinterpret_cast<Unreal::UObject*>(pair.First)->GetName()) != type)
            continue;

        auto* entries = reinterpret_cast<Unreal::TArray<UUWEPrimaryDataAssetBase*>*>(&pair.Second.Assets);
        entries->Add(asset);
        return;
    }
    Log::Warning("Failed to register data asset to registry: {}", type);
}
