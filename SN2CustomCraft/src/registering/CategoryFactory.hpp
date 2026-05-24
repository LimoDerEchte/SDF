//
// Created by Limo on 23/05/2026.
//

#pragma once

#include <string>

#include "SDK/UWECrafting_classes.hpp"

using namespace SDK;

class CategoryFactory {
    std::string categoryId, categoryName, categoryDescription;
    ECrafterType crafterType;

    UUWECraftingRecipeCategory* categoryParent = nullptr;
    TSoftObjectPtr<UTexture2D> categoryTexture;

public:
    CategoryFactory(std::string categoryId, std::string categoryName, std::string categoryDescription, ECrafterType crafterType);

    bool setParent(const std::string &categoryId);
    bool setParent(UUWECraftingRecipeCategory *category);

    bool setIconFromItem(const std::string &itemId);
    bool setIconFromItem(const UUWEItemType *item);
    bool setIcon(UTexture2D *icon);

    [[nodiscard]] UUWECraftingRecipeCategory* registerCategory() const;
};
