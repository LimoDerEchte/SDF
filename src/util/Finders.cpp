//
// Created by Limo on 28/05/2026.
//

#include "Finders.hpp"

#include "UObjectGlobals.hpp"
#include "UObject.hpp"

using namespace SDK;

template <UObjectDerived T>
T *Finders::findObject(const std::wstring &typeName, const std::string &fullName) {
    const auto item = RC::Unreal::UObjectGlobals::FindObject(typeName.c_str(), UtfN::StringToWString(fullName).c_str());
    return reinterpret_cast<T*>(item);
}

template<UObjectDerived T>
bool Finders::tryFindObject(const std::wstring &typeName, const std::string &fullName, T **outObject) {
    *outObject = findObject<T>(typeName, fullName);
    return *outObject != nullptr;
}

uintptr_t Finders::searchFirstOfInternal(const std::string &typeId) {
    const auto item = RC::Unreal::UObjectGlobals::FindFirstOf(UtfN::StringToWString(typeId).c_str());
    return reinterpret_cast<uintptr_t>(item);
}

UUWEItemType *Finders::searchItem(const std::string &itemId) {
    return findObject<UUWEItemType>(L"UWEItemType", "DA_" + itemId + "_ItemType");
}

UUWECraftingRecipe *Finders::searchRecipe(const std::string &recipeId) {
    UUWECraftingRecipe *recipe = nullptr;
    if (!tryFindObject(L"UWECraftingRecipe", "DA_" + recipeId + "Recipe", &recipe) && !recipeId.ends_with("_"))
        recipe = searchRecipe(recipeId + "_");
    return recipe;
}

UUWECraftingRecipeCategory *Finders::searchRecipeCategory(const std::string &categoryId) {
    return findObject<UUWECraftingRecipeCategory>(L"UWECraftingRecipeCategory", "DA_" + categoryId);
}

UUWEScanData *Finders::searchScanData(const std::string &scanId) {
    return findObject<UUWEScanData>(L"UWEScanData", "DA_" + scanId + "_ScanData");
}

UUWEDatabankEntry *Finders::searchDatabankEntry(const std::string &scanId) {
    return findObject<UUWEDatabankEntry>(L"UWEDatabankEntry", "DA_" + scanId + "_DatabankEntry");
}

UUWEStoryGoal *Finders::searchStoryGoal(const std::string &goalId) {
    UUWEStoryGoal *goal = nullptr;
    if (!tryFindObject(L"UWEStoryGoal", "DA_StoryGoal_" + goalId,  &goal))
        if (!tryFindObject(L"UWEStoryGoal", "DA_" + goalId + "_StoryGoal",  &goal))
            goal = findObject<UUWEStoryGoal>(L"UWEStoryGoal", "DA_" + goalId + "_SG");
    return goal;
}

UUWEStoryGoalRule *Finders::searchStoryGoalRule(const std::string &ruleId) {
    return findObject<UUWEStoryGoalRule>(L"UWEStoryGoalRule", "DA_" + ruleId);
}

USN2BuilderConstructActionData *Finders::searchBuilderAction(const std::string &dataId) {
    return findObject<USN2BuilderConstructActionData>(L"SN2BuilderConstructActionData", "DA_" + dataId);
}

UUWEBioAbilityData *Finders::searchBioAbilityData(const std::string &abilityName) {
    return findObject<UUWEBioAbilityData>(L"UWEBioAbilityData", "DA_" + abilityName + "_BioAbilityData");
}

UUWECrafterComponent *Finders::searchCrafterComponent(const std::string &componentPath) {
    const std::string trueExpr = "/Game/Blueprints/" + componentPath;
    const auto item = RC::Unreal::UObjectGlobals::StaticFindObject(nullptr, nullptr, UtfN::StringToWString(trueExpr).c_str());
    return reinterpret_cast<UUWECrafterComponent*>(item);
}

UTexture2D *Finders::searchTexture(const std::string &textureName) {
    return findObject<UTexture2D>(L"UTexture2D", textureName);
}
