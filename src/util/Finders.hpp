//
// Created by Limo on 28/05/2026.
//

#pragma once

#include "sdk/Types.hpp"

template<typename T>
concept UObjectDerived = std::is_base_of_v<UObject, T>;

class Finders {
    template <UObjectDerived T>
    static T* findObject(const std::wstring& typeName, const std::string &fullName);

    template <UObjectDerived T>
    static bool tryFindObject(const std::wstring& typeName, const std::string &fullName, T** outObject);

    static UObject* searchFirstOfInternal(const std::string &typeId);

public:
    template <UObjectDerived T>
    static T* searchFirstOf(const std::string &typeId) {
        return reinterpret_cast<T*>(searchFirstOfInternal(typeId));
    }

    static UUWEItemType *searchItem(const std::string &itemId);
    static UUWECraftingRecipe *searchRecipe(const std::string &recipeId);
    static UUWECraftingRecipeCategory *searchRecipeCategory(const std::string &categoryId);
    static UUWEScanData *searchScanData(const std::string &scanId);
    static UUWEDatabankEntry *searchDatabankEntry(const std::string &scanId);
    static UUWEStoryGoal *searchStoryGoal(const std::string &goalId);
    static UUWEStoryGoalRule *searchStoryGoalRule(const std::string &ruleId);
    static USN2BuilderConstructActionData *searchBuilderAction(const std::string &dataId);
    //static SDK::UUWEBioAbilityData *searchBioAbilityData(const std::string &abilityName);
    static UTexture2D *searchTexture(const std::string &textureName);
};
