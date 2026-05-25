//
// Created by Limo on 20/05/2026.
//

#pragma once

#include "SDK/UWECrafting_classes.hpp"
#include "SDK/Subnautica2_classes.hpp"
#include "polyhook2/Detour/x64Detour.hpp"
#include "Containers/Array.hpp"

using getRecipeT = RC::Unreal::TArray<SDK::UUWECraftingRecipe*>(*)();
using getActionsT = RC::Unreal::TArray<SDK::USN2BuilderActionData*>(*)();

class Hooks {
    static getRecipeT originalGetRecipes;
    static getActionsT originalGetActions;

    static std::unique_ptr<PLH::Detour> getRecipesHook;
    static std::unique_ptr<PLH::Detour> getActionsHook;

    static RC::Unreal::TArray<SDK::UUWECraftingRecipe*> GetRecipesHook();
    static RC::Unreal::TArray<SDK::USN2BuilderActionData*> GetActionsHook();

    static uintptr_t ScanCall(uintptr_t address, int ordinal);

public:
    static void RegisterHooks();
    static void UnregisterHooks();
};
