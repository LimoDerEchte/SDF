//
// Created by Limo on 20/05/2026.
//

#include "Hooks.hpp"

#include <memory>

#include "util/Log.hpp"
#include "RecipeFactory.hpp"
#include "polyhook2/Exceptions/AVehHook.hpp"
#include "SDK/Subnautica2_classes.hpp"
#include "Hooks/Hooks.hpp"

#include "UnrealDef.hpp"

using namespace SDK;
using namespace RC;
using namespace Unreal;

getRecipeT Hooks::originalGetRecipes = nullptr;
getActionsT Hooks::originalGetActions = nullptr;

std::unique_ptr<PLH::Detour> Hooks::getRecipesHook = nullptr;
std::unique_ptr<PLH::Detour> Hooks::getActionsHook = nullptr;

Unreal::TArray<UUWECraftingRecipe*> Hooks::GetRecipesHook() {
    auto recipes = originalGetRecipes();
    recipes.ResizeTo(static_cast<int32_t>(recipes.Num() + RecipeFactory::registeredRecipes.size()));

    for (const UUWECraftingRecipe* recipe : RecipeFactory::registeredRecipes) {
        recipes.Add(const_cast<UUWECraftingRecipe*>(recipe));
    }
    return recipes;
}

Unreal::TArray<USN2BuilderActionData*> Hooks::GetActionsHook() {
    auto actions = originalGetActions();
    // TODO: Add actions
    Log::Verbose("GetActionsHook called");
    return actions;
}

uintptr_t Hooks::ScanCall(uintptr_t address, int ordinal) {
    while (true) {
        if (*reinterpret_cast<uint8_t*>(address) == 0xE8) {
            ordinal--;
            if (ordinal > 0) {
                address += 5;
                continue;
            }
            const int32_t rel = *reinterpret_cast<int32_t*>(address + 1);
            return address + rel + 5;
        }
        address++;
    }
}

void Hooks::RegisterHooks() {
    Log::Verbose("Scanning internal functions");

    const auto recipeFunc = USN2AssetRegistry::StaticClass()->GetFunction("SN2AssetRegistry", "GetAllCraftingRecipes");
    const auto recipeFuncPtr = reinterpret_cast<uintptr_t>(*recipeFunc->ExecFunction);
    const auto recipeInternalPtr = ScanCall(recipeFuncPtr, 1);
    Log::Verbose("Found recipe registry getter at {:p}", reinterpret_cast<void*>(recipeInternalPtr));

    const auto actionFunc = USN2AssetRegistry::StaticClass()->GetFunction("SN2AssetRegistry", "GetAllBuilderActions");
    const auto actionFuncPtr = reinterpret_cast<uintptr_t>(*actionFunc->ExecFunction);
    const auto actionInternalPtr = ScanCall(actionFuncPtr, 1);
    Log::Verbose("Found builder action registry getter at {:p}", reinterpret_cast<void*>(actionInternalPtr));

    Log::Verbose("Hooking internal functions");

    getRecipesHook = std::make_unique<PLH::x64Detour>(
        recipeInternalPtr,
        reinterpret_cast<uint64_t>(&GetRecipesHook),
        reinterpret_cast<uint64_t*>(&originalGetRecipes)
    );

    if (getRecipesHook->hook())
        Log::Verbose("Successfully hooked recipe registry");
    else
        Log::Error("Failed to hook recipe registry!");

    getActionsHook = std::make_unique<PLH::x64Detour>(
        actionInternalPtr,
        reinterpret_cast<uint64_t>(&GetActionsHook),
        reinterpret_cast<uint64_t*>(&originalGetActions)
    );

    if (getActionsHook->hook())
        Log::Verbose("Successfully hooked builder action registry");
    else
        Log::Error("Failed to hook builder action registry!");
}

void Hooks::UnregisterHooks() {
    if (getRecipesHook->unHook())
        Log::Verbose("Successfully unhooked recipe registry");
    else
        Log::Error("Failed to unhook recipe registry!");

    if (getActionsHook->unHook())
        Log::Verbose("Successfully unhooked builder action registry");
    else
        Log::Error("Failed to unhook builder action registry!");
}
