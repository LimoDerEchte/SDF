//
// Created by Limo on 21/05/2026.
//

#include "RecipeFactory.hpp"

#include "util/Log.hpp"
#include "UObjectGlobals.hpp"

using namespace RC;
using namespace Unreal;

using EF = SDK::EObjectFlags;

std::vector<UUWECraftingRecipe*> RecipeFactory::registeredRecipes;

UUWEItemType *RecipeFactory::searchItem(const std::string &itemId) {
    const std::string trueExpr = "DA_" + itemId + "_ItemType";
    const auto item = UObjectGlobals::FindObject(L"UWEItemType", UtfN::StringToWString(trueExpr).c_str());
    return reinterpret_cast<UUWEItemType*>(item);
}

UUWECraftingRecipeCategory * RecipeFactory::searchRecipeCategory(const std::string &categoryId) {
    const std::string trueExpr = "DA_" + categoryId;
    const auto item = UObjectGlobals::FindObject(L"UWECraftingRecipeCategory", UtfN::StringToWString(trueExpr).c_str());
    return reinterpret_cast<UUWECraftingRecipeCategory*>(item);
}

RecipeFactory::RecipeFactory(std::string recipeId, std::string recipeName, std::string recipeDescription)
    : recipeId(std::move(recipeId)), recipeName(std::move(recipeName)), recipeDescription(std::move(recipeDescription)) {
}

bool RecipeFactory::setCategory(const std::string &categoryId) {
    return setCategory(searchRecipeCategory(categoryId));
}

bool RecipeFactory::setCategory(UUWECraftingRecipeCategory *category) {
    if (category == nullptr)
        return false;
    recipeCategory = category;
    return true;
}

bool RecipeFactory::addIngredient(const std::string &itemId, const int32_t amount) {
    return addIngredient(searchItem(itemId), amount);
}

bool RecipeFactory::addIngredient(UUWEItemType *item, const int32_t amount) {
    if (item == nullptr || amount < 1)
        return false;
    ingredients.emplace_back(FCraftingRecipeRequirement{
        .ItemType = UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(item),
        .NumItems = amount,
    });
    return true;
}

bool RecipeFactory::addOutput(const std::string &itemId, const int32_t amount) {
    return addOutput(searchItem(itemId), amount);
}

bool RecipeFactory::addOutput(UUWEItemType *item, const int32_t amount) {
    if (item == nullptr || amount < 1)
        return false;
    outputs.emplace_back(FCraftingRecipeOutput{
        .ItemType = UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(item),
        .NumItems = amount,
        .CollisionCheckSize = SDK::FVector(0, 0, 0),
    });
    return true;
}

void RecipeFactory::registerRecipe() const {
    const auto base = reinterpret_cast<UUWECraftingRecipe*>(UObjectGlobals::FindObject(L"UWECraftingRecipe", L"DA_MetalSalvageRecipe"));
    if (base == nullptr)
        return;

    const auto recipe = reinterpret_cast<UUWECraftingRecipe*>(UGameplayStatics::SpawnObject(UUWECraftingRecipe::StaticClass(), base->Outer));
    if (recipe == nullptr)
        return;

    recipe->Name = UKismetStringLibrary::Conv_StringToName(UtfN::StringToWString(std::format("DA_{}_CustomCraftRecipe", recipeId)).c_str());
    recipe->Flags = EF::MarkAsRootSet | EF::Public | EF::Standalone | EF::Transactional | EF::WasLoaded | EF::LoadCompleted;

    recipe->Name_0 = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(recipeName).c_str());
    recipe->Description = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(recipeDescription).c_str());

    const auto defaultTex = reinterpret_cast<UTexture2D*>(UObjectGlobals::FindObject(L"Texture2D", L"T_DefaultImage"));
    recipe->Thumbnail = static_cast<TSoftObjectPtr<UTexture2D>>(UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(defaultTex));

    const auto requirements = reinterpret_cast<Unreal::TArray<FCraftingRecipeRequirement>*>(&recipe->Requirements);
    for (const auto& ingredient: ingredients) {
        requirements->Add(ingredient);
    }

    const auto output = reinterpret_cast<Unreal::TArray<FCraftingRecipeOutput>*>(&recipe->Output);
    for (const auto& out : outputs) {
        output->Add(out);
    }

    recipe->Category = recipeCategory == nullptr ? base->Category : static_cast<TSoftObjectPtr<UUWECraftingRecipeCategory>>
                                                    (UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(recipeCategory));

    registeredRecipes.push_back(recipe);
    Log::Verbose("Recipe registered: {}", recipeId);
}

std::vector<UUWECraftingRecipe*> RecipeFactory::getAllRegisteredRecipes() {
    return registeredRecipes;
}
