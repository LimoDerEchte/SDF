//
// Created by Limo on 20/05/2026.
//

#pragma once

#include <SDK/UWECrafting_classes.hpp>
#include "polyhook2/Detour/x64Detour.hpp"
#include "Containers/Array.hpp"

using namespace SDK;

using getRecipeT = RC::Unreal::TArray<UUWECraftingRecipe*>(*)();

class Hooks {
    static getRecipeT originalGetRecipes;

    static std::unique_ptr<PLH::Detour> getRecipesHook;

    static RC::Unreal::TArray<UUWECraftingRecipe*> GetRecipesHook();

    static uintptr_t ScanCall(uintptr_t address, int ordinal);

public:
    static void RegisterHooks();
    static void UnregisterHooks();
};
