//
// Created by Limo on 28/05/2026.
//

#pragma once

#include "SDK/Subnautica2_classes.hpp"
#include "SDK/UWECrafting_classes.hpp"
#include "SDK/UWEScanner_classes.hpp"

class Finders {
    static uintptr_t searchFirstOfInternal(const std::string &typeId);

public:
    template <typename T, typename = std::enable_if_t<std::is_base_of_v<SDK::UObject, T>>>
    static T* searchFirstOf(const std::string &typeId) {
        return reinterpret_cast<T*>(searchFirstOfInternal(typeId));
    }

    static SDK::UUWEItemType *searchItem(const std::string &itemId);
    static SDK::UUWECraftingRecipe *searchRecipe(const std::string &recipeId);
    static SDK::UUWECraftingRecipeCategory *searchRecipeCategory(const std::string &categoryId);
    static SDK::UUWEScanData *searchScanData(const std::string &scanId);
    static SDK::UUWEDatabankEntry *searchDatabankEntry(const std::string &scanId);
    static SDK::UUWEStoryGoal *searchStoryGoal(const std::string &goalId);
    static SDK::UUWEStoryGoalRule *searchStoryGoalRule(const std::string &ruleId);
    static SDK::USN2BuilderConstructActionData *searchBuilderAction(const std::string &dataId);
    static SDK::UUWECrafterComponent *searchComponent(const std::string &componentPath);
    static SDK::UTexture2D *findCicadaTexture();
};
