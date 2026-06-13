//
// Created by Limo on 20/05/2026.
//

#pragma once

#include "SDK/UWECrafting_classes.hpp"
#include "SDK/Subnautica2_classes.hpp"
#include "polyhook2/Detour/x64Detour.hpp"
#include "Containers/Array.hpp"

#define HookDefHeader(type, name) \
    using get##name##T = RC::Unreal::TArray<type*>(*)(); \
    static get##name##T originalGet##name; \
    static std::unique_ptr<PLH::Detour> get##name##Hook; \
    static RC::Unreal::TArray<type*> Get##name##Hook(); \

class Hooks {
    HookDefHeader(SDK::UUWECraftingRecipe, Recipes)
    HookDefHeader(SDK::USN2BuilderActionData, BuilderActions)

#ifdef DEVELOPMENT
    using getAssetsT = bool(*)(void*, const SDK::FARFilter*, RC::Unreal::TArray<SDK::FAssetData>*, bool);

    static getAssetsT originalGetAssets;
    static std::unique_ptr<PLH::Detour> getAssetsHook;
    static bool GetAssetsHook(void* self, const SDK::FARFilter* filter, RC::Unreal::TArray<SDK::FAssetData>* out, bool bSkipARFilteredAsset);
#endif

    static uintptr_t ScanCall(uintptr_t address, int ordinal);
    static uintptr_t ScanCallMultiPass(uintptr_t address, const std::vector<int>& ordinals);

public:
    static void RegisterHooks();
    static void UnregisterHooks();
};
