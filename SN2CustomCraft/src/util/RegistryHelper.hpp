//
// Created by Limo on 09/06/2026.
//

#pragma once

#include "UObjectGlobals.hpp"
#include "SDK/UWEAssetRegistry_classes.hpp"

class RegistryHelper {
    static void InitCache();

public:
    template <typename T, typename = std::enable_if_t<std::is_base_of_v<SDK::UObject, T>>>
    static T *StaticConstructTemplate(T *base, const std::string name, const std::optional<SDK::UObject*> overrideOuter = std::nullopt) {
        RC::Unreal::FStaticConstructObjectParameters params(
            reinterpret_cast<RC::Unreal::UClass*>(base->Class),
            reinterpret_cast<RC::Unreal::UObject*>(overrideOuter.value_or(base->Outer))
        );

        params.Name = RC::Unreal::FName(UtfN::StringToWString(name).c_str());
        params.SetFlags = static_cast<RC::Unreal::EObjectFlags>(
            SDK::EObjectFlags::MarkAsRootSet |
            SDK::EObjectFlags::Public |
            SDK::EObjectFlags::Standalone |
            SDK::EObjectFlags::Transactional |
            SDK::EObjectFlags::WasLoaded |
            SDK::EObjectFlags::LoadCompleted);
        return reinterpret_cast<T*>(RC::Unreal::UObjectGlobals::StaticConstructObject(params));
    }
    static void AddToRegistry(SDK::UUWEPrimaryDataAssetBase* asset, const std::string &type);
};
