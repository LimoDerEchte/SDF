//
// Created by Limo on 20/05/2026.
//

#pragma once

#include "sdk/Types.hpp"
#include "polyhook2/Detour/x64Detour.hpp"

#define HookDefHeader(type, name) \
    using get##name##T = TArray<type*>(*)(); \
    static get##name##T originalGet##name; \
    static std::unique_ptr<PLH::Detour> get##name##Hook; \
    static TArray<type*> Get##name##Hook(); \

class Hooks {
    HookDefHeader(UUWECraftingRecipe, Recipes)
    HookDefHeader(USN2BuilderActionData, BuilderActions)
    HookDefHeader(UUWEDatabankEntry, DatabankEntries)

    static uintptr_t ScanCall(uintptr_t address, int ordinal);
    static uintptr_t ScanCallMultiPass(uintptr_t address, const std::vector<int>& ordinals);

public:
    static void RegisterHooks();
    static void UnregisterHooks();
};
