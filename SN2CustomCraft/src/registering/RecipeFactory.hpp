//
// Created by Limo on 21/05/2026.
//

#pragma once

#include <vector>

#include "SDK/UWECrafting_classes.hpp"
#include "SDK/UWEInventory_classes.hpp"

using namespace SDK;

class RecipeFactory {
    static std::vector<UUWECraftingRecipe*> registeredRecipes;

    std::string recipeId, recipeName, recipeDescription;

    UUWECraftingRecipeCategory* recipeCategory = nullptr;
    TSoftObjectPtr<UTexture2D> recipeTexture;

    std::vector<FCraftingRecipeRequirement> ingredients{};
    std::vector<FCraftingRecipeOutput> outputs{};

    static UUWEItemType *searchItem(const std::string &itemId);
    static UUWECraftingRecipeCategory *searchRecipeCategory(const std::string &categoryId);

    friend class CategoryFactory;

public:
    explicit RecipeFactory(std::string recipeId, std::string recipeName, std::string recipeDescription);

    bool setCategory(const std::string &categoryId);
    bool setCategory(UUWECraftingRecipeCategory *category);

    bool setIconFromItem(const std::string &itemId);
    bool setIconFromItem(const UUWEItemType *item);
    bool setIcon(UTexture2D *icon);

    bool addIngredient(const std::string &itemId, int32_t amount);
    bool addIngredient(UUWEItemType *item, int32_t amount);

    bool addOutput(const std::string &itemId, int32_t amount);
    bool addOutput(UUWEItemType *item, int32_t amount);

    UUWECraftingRecipe* registerRecipe() const;

    static std::vector<UUWECraftingRecipe*> getAllRegisteredRecipes();
};
