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

void RegistryHelper::AddToRegistry(UUWEPrimaryDataAssetBase *asset, const std::string &type) {
    InitCache();

    for (auto& pair : assetRegistrySubsystem->CachedDataAssets) {
        if (!pair.First->GetFullName().contains(type))
            continue;

        auto* entries = reinterpret_cast<Unreal::TArray<UUWEPrimaryDataAssetBase*>*>(&pair.Second.Assets);
        entries->Add(asset);
        Log::Warning("Test {}", entries->Num());
        return;
    }
    Log::Warning("Failed to register data asset to registry");
}
