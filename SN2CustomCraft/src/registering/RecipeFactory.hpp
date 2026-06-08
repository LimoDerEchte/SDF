//
// Created by Limo on 21/05/2026.
//

#pragma once

#include <map>
#include <vector>

#include "SDK/UWECrafting_classes.hpp"
#include "SDK/UWEInventory_classes.hpp"
#include "SDK/UWEScanner_classes.hpp"

class RecipeFactory {
    static std::vector<SDK::UUWECraftingRecipe*> registeredRecipes;
    static std::vector<SDK::UUWECraftingRecipe*> registeredRecipesLifePod;

    bool modifyMode = false;
    bool availableInLifePodModify = false;
    bool removeRequirementsModify = false;

    std::string recipeId, recipeName, recipeDescription;

    bool availableInLifePod = false;
    SDK::UUWECraftingRecipeCategory* recipeCategory = nullptr;
    bool recipeTextureModified = false;
    SDK::TSoftObjectPtr<SDK::UTexture2D> recipeTexture;

    std::vector<SDK::FCraftingRecipeRequirement> ingredients{};
    std::vector<SDK::FCraftingRecipeOutput> outputs{};
    std::map<std::string, std::vector<SDK::FUWERecipeUnlockRuleEntry>> unlockingRules{};
    float craftingTime = 2.0f;

    friend class Hooks;

    static void unregisterAllRecipes();

public:
    explicit RecipeFactory(std::string recipeId, bool isModify);

    void setName(const std::string& recipeName);
    void setDescription(const std::string& recipeDescription);

    bool setCategory(const std::string &categoryId);
    bool setCategory(SDK::UUWECraftingRecipeCategory *category);

    bool setIconFromItem(const std::string &itemId);
    bool setIconFromItem(const SDK::UUWEItemType *item);
    bool setIcon(SDK::UTexture2D *icon);

    bool addIngredient(const std::string &itemId, int32_t amount);
    bool addIngredient(SDK::UUWEItemType *item, int32_t amount);

    bool addOutput(const std::string &itemId, int32_t amount);
    bool addOutput(SDK::UUWEItemType *item, int32_t amount);

    bool addUnlockingRequirementPickup(const std::string &ruleSet, const std::string &itemId);
    bool addUnlockingRequirementPickup(const std::string &ruleSet, SDK::UUWEItemType *item);

    bool addUnlockingRequirementScanData(const std::string &ruleSet, const std::string &dataId);
    bool addUnlockingRequirementScanData(const std::string &ruleSet, SDK::UUWEScanData* data);

    bool addUnlockingRequirement(const std::string &ruleSet, SDK::FUWERecipeUnlockRuleEntry entry);
    void modifyRemoveRequirements();

    void setCraftingTime(float time);
    void setAvailableInLifepod(bool available);

    [[nodiscard]] SDK::UUWECraftingRecipe* registerRecipe() const;
};
