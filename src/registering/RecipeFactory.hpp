//
// Created by Limo on 21/05/2026.
//

#pragma once

#include <map>
#include <vector>

#include "sdk/Types.hpp"

class RecipeFactory {
    static std::vector<UUWECraftingRecipe*> registeredRecipes;
    static std::vector<UUWECraftingRecipe*> registeredRecipesLifePod;

    bool modifyMode = false;
    bool availableInLifePodModify = false;
    bool removeRequirementsModify = false;

    std::string recipeId, recipeName, recipeDescription;
    int orderingIndex = 0;
    bool orderingIndexModify = false;

    bool availableInLifePod = false;
    UUWECraftingRecipeCategory* recipeCategory = nullptr;
    bool recipeTextureModified = false;
    TSoftObjectPtr<UTexture2D> recipeTexture;

    std::vector<FCraftingRecipeRequirement> ingredients{};
    std::vector<FCraftingRecipeOutput> outputs{};
    std::map<std::string, std::vector<FUWERecipeUnlockRuleEntry>> unlockingRules{};
    float craftingTime = 2.0f;

    friend class Hooks;

public:
    explicit RecipeFactory(std::string recipeId, bool isModify);

    void setName(const std::string& recipeName);
    void setDescription(const std::string& recipeDescription);

    bool setCategory(const std::string &categoryId);
    bool setCategory(UUWECraftingRecipeCategory *category);

    bool setIconFromItem(const std::string &itemId);
    bool setIconFromItem(const UUWEItemType *item);
    bool setIcon(UTexture2D *icon);
    void setIcon(const TSoftObjectPtr<UTexture2D> &icon);

    bool addIngredient(const std::string &itemId, int32_t amount);
    bool addIngredient(UUWEItemType *item, int32_t amount);

    bool addOutput(const std::string &itemId, int32_t amount);
    bool addOutput(UUWEItemType *item, int32_t amount);

    bool addUnlockingRequirementPickup(const std::string &ruleSet, const std::string &itemId);
    bool addUnlockingRequirementPickup(const std::string &ruleSet, UUWEItemType *item);

    bool addUnlockingRequirementScanData(const std::string &ruleSet, const std::string &dataId);
    bool addUnlockingRequirementScanData(const std::string &ruleSet, UUWEScanData* data);

    bool addUnlockingRequirement(const std::string &ruleSet, FUWERecipeUnlockRuleEntry entry);
    void modifyRemoveRequirements();

    void setCraftingTime(float time);
    void setAvailableInLifepod(bool available);
    void setOrderingIndex(int orderingIndex);

    [[nodiscard]] UUWECraftingRecipe* registerRecipe() const;
};
