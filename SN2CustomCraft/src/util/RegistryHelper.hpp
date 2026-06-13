//
// Created by Limo on 09/06/2026.
//

#pragma once

#include "SDK/UWEAssetRegistry_classes.hpp"

class RegistryHelper {
    static void InitCache();

public:
    template <typename T, typename = std::enable_if_t<std::is_base_of_v<SDK::UObject, T>>>
    static T* StaticConstructTemplate(T* base, std::string name);
    static void AddToRegistry(SDK::UUWEPrimaryDataAssetBase* asset, const std::string &type);
};
