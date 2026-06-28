//
// Created by Limo on 23/05/2026.
//

#pragma once

#include <string>

#include "SDK/UWECrafting_classes.hpp"

class CategoryFactory {
    std::string categoryId, categoryName, categoryDescription;
    SDK::ECrafterType crafterType;
    bool rootCategory = true;

    int orderingIndex;
    bool orderingIndexModify;

    bool showWhenEmpty = false;
    bool showWhenEmptyModify = false;

    bool modifyMode = false;
    bool modifyCrafterType = false;

    SDK::UUWECraftingRecipeCategory* categoryParent = nullptr;
    bool categoryTextureModified = false;
    SDK::TSoftObjectPtr<SDK::UTexture2D> categoryTexture;

public:
    CategoryFactory(std::string categoryId, bool modifyMode);

    void setName(const std::string &categoryName);
    void setDescription(const std::string &categoryDescription);

    void setCrafterType(SDK::ECrafterType crafterType);
    void setOrderingIndex(int orderingIndex);
    void setShowWhenEmpty(bool showWhenEmpty);

    bool setParent(const std::string &categoryId);
    bool setParent(SDK::UUWECraftingRecipeCategory *category);

    bool setIconFromItem(const std::string &itemId);
    bool setIconFromItem(const SDK::UUWEItemType *item);
    bool setIcon(SDK::UTexture2D *icon);
    void setIcon(const SDK::TSoftObjectPtr<SDK::UTexture2D> &icon);

    [[nodiscard]] SDK::UUWECraftingRecipeCategory* registerCategory() const;
};
