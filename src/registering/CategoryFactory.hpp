//
// Created by Limo on 23/05/2026.
//

#pragma once

#include <string>

#include "sdk/Types.hpp"

class CategoryFactory {
    std::string categoryId, categoryName, categoryDescription;
    ECrafterType crafterType;
    bool rootCategory = true;

    int orderingIndex;
    bool orderingIndexModify;

    bool showWhenEmpty = false;
    bool showWhenEmptyModify = false;

    bool modifyMode = false;
    bool modifyCrafterType = false;

    UUWECraftingRecipeCategory* categoryParent = nullptr;
    bool categoryTextureModified = false;
    UTexture2D* categoryTexture;

public:
    CategoryFactory(std::string categoryId, bool modifyMode);

    void setName(const std::string &categoryName);
    void setDescription(const std::string &categoryDescription);

    void setCrafterType(ECrafterType crafterType);
    void setOrderingIndex(int orderingIndex);
    void setShowWhenEmpty(bool showWhenEmpty);

    bool setParent(const std::string &categoryId);
    bool setParent(UUWECraftingRecipeCategory *category);

    bool setIconFromItem(const std::string &itemId);
    bool setIconFromItem(UUWEItemType *item);
    bool setIcon(UTexture2D *icon);

    [[nodiscard]] UUWECraftingRecipeCategory* registerCategory() const;
};
