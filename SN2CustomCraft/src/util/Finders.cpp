//
// Created by Limo on 28/05/2026.
//

#include "Finders.hpp"

#include "UObject.hpp"
#include "UObjectGlobals.hpp"

using namespace SDK;

UUWEItemType *Finders::searchItem(const std::string &itemId) {
    const std::string trueExpr = "DA_" + itemId + "_ItemType";
    const auto item = RC::Unreal::UObjectGlobals::FindObject(L"UWEItemType", UtfN::StringToWString(trueExpr).c_str());
    return reinterpret_cast<UUWEItemType*>(item);
}

UUWECraftingRecipe *Finders::searchRecipe(const std::string &recipeId) {
    std::string trueExpr = "DA_" + recipeId + "Recipe";
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

USN2BuilderActionData *Finders::searchBuilderAction(const std::string &dataId) {
    const std::string trueExpr = "DA_" + dataId;
    const auto item = RC::Unreal::UObjectGlobals::FindObject(L"SN2BuilderConstructActionData", UtfN::StringToWString(trueExpr).c_str());
    return reinterpret_cast<USN2BuilderActionData*>(item);
}

UUWECrafterComponent *Finders::searchComponent(const std::string &componentPath) {
    const std::string trueExpr = "/Game/Blueprints/" + componentPath;
    const auto item = RC::Unreal::UObjectGlobals::StaticFindObject(nullptr, nullptr, UtfN::StringToWString(trueExpr).c_str());
    return reinterpret_cast<UUWECrafterComponent*>(item);
}

UTexture2D *Finders::findCicadaTexture() {
    return reinterpret_cast<UTexture2D*>(RC::Unreal::UObjectGlobals::FindObject(L"Texture2D", L"T_DefaultImage"));
}
