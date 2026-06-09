//
// Created by Limo on 28/05/2026.
//

#pragma once

#include "SDK/Subnautica2_classes.hpp"
#include "SDK/UWECrafting_classes.hpp"
#include "SDK/UWEScanner_classes.hpp"

class Finders {
public:
    static SDK::UUWEItemType *searchItem(const std::string &itemId);
    static SDK::UUWECraftingRecipe *searchRecipe(const std::string &recipeId);
    static SDK::UUWECraftingRecipeCategory *searchRecipeCategory(const std::string &categoryId);
    static SDK::UUWEScanData *searchScanData(const std::string &scanId);
    static SDK::USN2BuilderActionData *searchBuilderAction(const std::string &dataId);
    static SDK::UUWECrafterComponent *searchComponent(const std::string &componentPath);
    static SDK::UTexture2D *findCicadaTexture();
};
