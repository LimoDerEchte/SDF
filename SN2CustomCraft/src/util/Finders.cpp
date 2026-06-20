//
// Created by Limo on 28/05/2026.
//

#include "util/Finders.hpp"

#include "UObjectGlobals.hpp"
#include "UObject.hpp"

using namespace SDK;

uintptr_t Finders::searchFirstOfInternal(const std::string &typeId) {
    const auto item = RC::Unreal::UObjectGlobals::FindFirstOf(UtfN::StringToWString(typeId).c_str());
    return reinterpret_cast<uintptr_t>(item);
}

UUWEItemType *Finders::searchItem(const std::string &itemId) {
    const std::string trueExpr = "DA_" + itemId + "_ItemType";
    const auto item = RC::Unreal::UObjectGlobals::FindObject(L"UWEItemType", UtfN::StringToWString(trueExpr).c_str());
    return reinterpret_cast<UUWEItemType*>(item);
}

UUWECraftingRecipe *Finders::searchRecipe(const std::string &recipeId) {
    std::string trueExpr = recipeId.contains("Recipe") ? "DA_" + recipeId : "DA_" + recipeId + "Recipe";
    auto item = RC::Unreal::UObjectGlobals::FindObject(L"UWECraftingRecipe", UtfN::StringToWString(trueExpr).c_str());
    if (item == nullptr) {
        trueExpr = "DA_" + recipeId + "_Recipe";
        item = RC::Unreal::UObjectGlobals::FindObject(L"UWECraftingRecipe", UtfN::StringToWString(trueExpr).c_str());
    }
    return reinterpret_cast<UUWECraftingRecipe*>(item);
}

UUWECraftingRecipeCategory *Finders::searchRecipeCategory(const std::string &categoryId) {
    const std::string trueExpr = "DA_" + categoryId;
    const auto item = RC::Unreal::UObjectGlobals::FindObject(L"UWECraftingRecipeCategory", UtfN::StringToWString(trueExpr).c_str());
    return reinterpret_cast<UUWECraftingRecipeCategory*>(item);
}

UUWEScanData *Finders::searchScanData(const std::string &scanId) {
    const std::string trueExpr = "DA_" + scanId + "_ScanData";
    const auto item = RC::Unreal::UObjectGlobals::FindObject(L"UWEScanData", UtfN::StringToWString(trueExpr).c_str());
    return reinterpret_cast<UUWEScanData*>(item);
}

UUWEDatabankEntry * Finders::searchDatabankEntry(const std::string &scanId) {
    const std::string trueExpr = "DA_" + scanId + "_DatabankEntry";
    const auto item = RC::Unreal::UObjectGlobals::FindObject(L"UWEDatabankEntry", UtfN::StringToWString(trueExpr).c_str());
    return reinterpret_cast<UUWEDatabankEntry*>(item);
}

UUWEStoryGoal *Finders::searchStoryGoal(const std::string &goalId) {
    std::string trueExpr = "DA_StoryGoal_" + goalId;
    auto item = RC::Unreal::UObjectGlobals::FindObject(L"UWEStoryGoal", UtfN::StringToWString(trueExpr).c_str());
    if (item == nullptr) {
        trueExpr = "DA_" + goalId + "_StoryGoal";
        item = RC::Unreal::UObjectGlobals::FindObject(L"UWEStoryGoal", UtfN::StringToWString(trueExpr).c_str());
    }
    return reinterpret_cast<UUWEStoryGoal*>(item);
}

UUWEStoryGoalRule *Finders::searchStoryGoalRule(const std::string &ruleId) {
    const std::string trueExpr = "DA_" + ruleId;
    const auto item = RC::Unreal::UObjectGlobals::FindObject(L"UWEStoryGoalRule", UtfN::StringToWString(trueExpr).c_str());
    return reinterpret_cast<UUWEStoryGoalRule*>(item);
}

USN2BuilderConstructActionData *Finders::searchBuilderAction(const std::string &dataId) {
    const std::string trueExpr = "DA_" + dataId;
    const auto item = RC::Unreal::UObjectGlobals::FindObject(L"SN2BuilderConstructActionData", UtfN::StringToWString(trueExpr).c_str());
    return reinterpret_cast<USN2BuilderConstructActionData*>(item);
}

UUWECrafterComponent *Finders::searchComponent(const std::string &componentPath) {
    const std::string trueExpr = "/Game/Blueprints/" + componentPath;
    const auto item = RC::Unreal::UObjectGlobals::StaticFindObject(nullptr, nullptr, UtfN::StringToWString(trueExpr).c_str());
    return reinterpret_cast<UUWECrafterComponent*>(item);
}

UTexture2D *Finders::findCicadaTexture() {
    return reinterpret_cast<UTexture2D*>(RC::Unreal::UObjectGlobals::FindObject(L"Texture2D", L"T_DefaultImage"));
}
