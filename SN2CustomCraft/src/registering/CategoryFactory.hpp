//
// Created by Limo on 23/05/2026.
//

#pragma once

#include <string>

#include "SDK/UWECrafting_classes.hpp"

class CategoryFactory {
    static std::vector<SDK::UUWECraftingRecipeCategory*> registeredCategories;

    std::string categoryId, categoryName, categoryDescription;
    SDK::ECrafterType crafterType;

    SDK::UUWECraftingRecipeCategory* categoryParent = nullptr;
    SDK::TSoftObjectPtr<SDK::UTexture2D> categoryTexture;

    friend class SN2CustomCraft;

    static void unregisterAllCategories();

public:
    CategoryFactory(std::string categoryId, std::string categoryName, std::string categoryDescription, SDK::ECrafterType crafterType);

    bool setParent(const std::string &categoryId);
    bool setParent(SDK::UUWECraftingRecipeCategory *category);

    bool setIconFromItem(const std::string &itemId);
    bool setIconFromItem(const SDK::UUWEItemType *item);
    bool setIcon(SDK::UTexture2D *icon);

    [[nodiscard]] SDK::UUWECraftingRecipeCategory* registerCategory() const;
};
