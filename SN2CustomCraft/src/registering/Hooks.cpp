//
// Created by Limo on 20/05/2026.
//

#include "Hooks.hpp"

#include <memory>

#include "util/Log.hpp"
#include "RecipeFactory.hpp"
#include "polyhook2/Exceptions/AVehHook.hpp"
#include "SDK/Subnautica2_classes.hpp"

getRecipeT Hooks::originalGetRecipes = nullptr;

std::unique_ptr<PLH::Detour> Hooks::getRecipesHook = nullptr;

RC::Unreal::TArray<UUWECraftingRecipe*> Hooks::GetRecipesHook() {
    const auto registered = RecipeFactory::getAllRegisteredRecipes();
    auto recipes = originalGetRecipes();

    recipes.ResizeTo(static_cast<int32_t>(recipes.Num() + registered.size()));
    for (const UUWECraftingRecipe* recipe : registered) {
        recipes.Add(const_cast<UUWECraftingRecipe*>(recipe));
    }
    return recipes;
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
    Log::Verbose("Hooking recipe registry");

    const auto recipeFunc = USN2AssetRegistry::StaticClass()->GetFunction("SN2AssetRegistry", "GetAllCraftingRecipes");
    const auto funcPtr = reinterpret_cast<uintptr_t>(*recipeFunc->ExecFunction);
    const auto internalPtr = ScanCall(funcPtr, 1);

    Log::Verbose("Found recipe registry getter at {:p}", reinterpret_cast<void*>(internalPtr));

    getRecipesHook = std::make_unique<PLH::x64Detour>(
        internalPtr,
        reinterpret_cast<uint64_t>(&GetRecipesHook),
        reinterpret_cast<uint64_t*>(&originalGetRecipes)
    );

    if (getRecipesHook->hook())
        Log::Verbose("Successfully hooked recipe registry");
    else
        Log::Error("Failed to hook recipe registry!");
}
