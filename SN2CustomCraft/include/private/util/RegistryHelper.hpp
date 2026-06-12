//
// Created by Limo on 09/06/2026.
//

#pragma once
#include "SDK/UWEAssetRegistry_classes.hpp"

class RegistryHelper {
    static void InitCache();

public:
    static void AddToRegistry(SDK::UUWEPrimaryDataAssetBase* asset, const std::string &type);
};
