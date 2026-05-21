//
// Created by Limo on 20/05/2026.
//

#pragma once

#include <SDK/UWECrafting_classes.hpp>
#include "polyhook2/Detour/x64Detour.hpp"

#define OFFSET_GET_RECIPES 0x6B737D0

using getRecipeT = UC::TArray<SDK::UUWECraftingRecipe*>(*)();

class Hooks {
    static getRecipeT originalGetRecipes;

    static std::unique_ptr<PLH::Detour> getRecipesHook;

    static UC::TArray<SDK::UUWECraftingRecipe*> GetRecipesHook();

    static uintptr_t ScanCall(uintptr_t address, int ordinal);

public:
    static void RegisterHooks();
};
