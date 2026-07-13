//
// Created by Limo on 09/06/2026.
//

#pragma once

#include "sdk/Types.hpp"

class RegistryHelper {
    static void InitCache();

    static uintptr_t StaticConstructTemplateInternal(UObject *base, const std::string &name, std::optional<UObject*> overrideOuter);

public:
    template <typename T, typename = std::enable_if_t<std::is_base_of_v<UObject, T>>>
    static T *StaticConstructTemplate(T *base, const std::string &name, const std::optional<UObject*> overrideOuter = std::nullopt) {
        return reinterpret_cast<T*>(StaticConstructTemplateInternal(base, name, overrideOuter));
    }
    static void AddToRegistry(UUWEPrimaryDataAssetBase* asset, const std::string &type);
};
