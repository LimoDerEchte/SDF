//
// Created by Limo on 21/05/2026.
//

#include "RecipeFactory.hpp"

#include "util/Log.hpp"
#include "UObjectGlobals.hpp"
#include "UObject.hpp"

using namespace SDK;
using namespace RC;
using namespace Unreal;

using EF = SDK::EObjectFlags;

std::vector<UUWECraftingRecipe*> RecipeFactory::registeredRecipes;
std::vector<UUWECraftingRecipe*> RecipeFactory::registeredRecipesLifePod;

UUWEItemType *RecipeFactory::searchItem(const std::string &itemId) {
    const std::string trueExpr = "DA_" + itemId + "_ItemType";
    const auto item = UObjectGlobals::FindObject(L"UWEItemType", UtfN::StringToWString(trueExpr).c_str());
    return reinterpret_cast<UUWEItemType*>(item);
}

UUWECraftingRecipeCategory *RecipeFactory::searchRecipeCategory(const std::string &categoryId) {
    const std::string trueExpr = "DA_" + categoryId;
    const auto item = UObjectGlobals::FindObject(L"UWECraftingRecipeCategory", UtfN::StringToWString(trueExpr).c_str());
    return reinterpret_cast<UUWECraftingRecipeCategory*>(item);
}

UUWEScanData *RecipeFactory::searchScanData(const std::string &scanId) {
    const std::string trueExpr = "DA_" + scanId + "_ScanData";
    const auto item = UObjectGlobals::FindObject(L"UWEScanData", UtfN::StringToWString(trueExpr).c_str());
    return reinterpret_cast<UUWEScanData*>(item);
}

void RecipeFactory::unregisterAllRecipes() {
    registeredRecipesLifePod.clear();
    for (const auto& registered_recipe : registeredRecipes)
        reinterpret_cast<Unreal::UObject*>(registered_recipe)->BeginDestroy();
    registeredRecipes.clear();
}

RecipeFactory::RecipeFactory(std::string recipeId, std::string recipeName, std::string recipeDescription)
    : recipeId(std::move(recipeId)), recipeName(std::move(recipeName)), recipeDescription(std::move(recipeDescription)) {

    const auto defaultTex = reinterpret_cast<UTexture2D*>(UObjectGlobals::FindObject(L"Texture2D", L"T_DefaultImage"));
    setIcon(defaultTex);
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

bool RecipeFactory::setIconFromItem(const std::string &itemId) {
    return setIconFromItem(searchItem(itemId));
}

bool RecipeFactory::setIconFromItem(const UUWEItemType *item) {
    if (item == nullptr)
        return false;
    recipeTexture = item->Thumbnail;
    return true;
}

bool RecipeFactory::setIcon(UTexture2D *icon) {
    if (icon == nullptr)
        return false;
    recipeTexture = static_cast<TSoftObjectPtr<UTexture2D>>(UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(icon));
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

bool RecipeFactory::addUnlockingRequirementPickup(const std::string &ruleSet, const std::string &itemId) {
    return addUnlockingRequirementPickup(ruleSet, searchItem(itemId));
}

bool RecipeFactory::addUnlockingRequirementPickup(const std::string &ruleSet, UUWEItemType *item) {
    if (item == nullptr)
        return false;
    return addUnlockingRequirement(ruleSet, FUWERecipeUnlockRuleEntry {
        .EventType = ERecipeEventTypes::OnPickup,
        .RequirementScope = ERequirementScope::PlayerSpecific,
        .RequiredCount = 1,
        .EventAsset = item
    });
}

bool RecipeFactory::addUnlockingRequirementScanData(const std::string &ruleSet, const std::string &dataId) {
    return addUnlockingRequirementScanData(ruleSet, searchScanData(dataId));
}

bool RecipeFactory::addUnlockingRequirementScanData(const std::string &ruleSet, UUWEScanData *data) {
    if (data == nullptr)
        return false;
    return addUnlockingRequirement(ruleSet, FUWERecipeUnlockRuleEntry {
        .EventType = ERecipeEventTypes::OnFullScan,
        .RequirementScope = ERequirementScope::PlayerSpecific,
        .RequiredCount = 1,
        .EventAsset = data
    });
}

bool RecipeFactory::addUnlockingRequirement(const std::string &ruleSet, FUWERecipeUnlockRuleEntry entry) {
    if (ruleSet.empty())
        return false;
    if (!unlockingRules.contains(ruleSet))
        unlockingRules.emplace(ruleSet, std::vector<FUWERecipeUnlockRuleEntry>());
    unlockingRules[ruleSet].emplace_back(entry);
    return true;
}

void RecipeFactory::setCraftingTime(const float time) {
    craftingTime = time;
}

void RecipeFactory::makeAvailableInLifePod() {
    availableInLifePod = true;
}

UUWECraftingRecipe* RecipeFactory::registerRecipe() const {
    const auto base = reinterpret_cast<UUWECraftingRecipe*>(UObjectGlobals::FindObject(L"UWECraftingRecipe", L"DA_MetalSalvageRecipe"));
    if (base == nullptr)
        return nullptr;

    const auto recipe = static_cast<UUWECraftingRecipe*>(UGameplayStatics::SpawnObject(UUWECraftingRecipe::StaticClass(), base->Outer));
    if (recipe == nullptr)
        return nullptr;

    recipe->Name = UKismetStringLibrary::Conv_StringToName(UtfN::StringToWString(std::format("DA_{}_CustomCraftRecipe", recipeId)).c_str());
    recipe->Flags = EF::MarkAsRootSet | EF::Public | EF::Standalone | EF::Transactional | EF::WasLoaded | EF::LoadCompleted;

    recipe->Name_0 = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(recipeName).c_str());
    recipe->Description = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(recipeDescription).c_str());

    recipe->Thumbnail = recipeTexture;
    recipe->Category = recipeCategory == nullptr ? base->Category : static_cast<TSoftObjectPtr<UUWECraftingRecipeCategory>>(UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(recipeCategory));
    recipe->CraftingTime = craftingTime;

    const auto requirements = reinterpret_cast<Unreal::TArray<FCraftingRecipeRequirement>*>(&recipe->Requirements);
    requirements->ResizeTo(requirements->Num() + static_cast<int32_t>(ingredients.size()));
    for (const auto& ingredient: ingredients) {
        requirements->Add(ingredient);
    }

    const auto output = reinterpret_cast<Unreal::TArray<FCraftingRecipeOutput>*>(&recipe->Output);
    output->ResizeTo(output->Num() + static_cast<int32_t>(outputs.size()));
    for (const auto& out : outputs) {
        output->Add(out);
    }

    const auto rules = reinterpret_cast<Unreal::TArray<FUWERecipeUnlockRules>*>(&recipe->UpdatedUnlockingRequirements);
    rules->ResizeTo(rules->Num() + static_cast<int32_t>(unlockingRules.size()));
    recipe->DefaultRecipeState = ERecipeState::Unlocked;
    for (const auto&[first, second]: unlockingRules) {
        auto rule = FUWERecipeUnlockRules {
            .RuleName = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(first).c_str()),
            .Entries = UC::TArray<FUWERecipeUnlockRuleEntry>(),
        };
        const auto entries = reinterpret_cast<Unreal::TArray<FUWERecipeUnlockRuleEntry>*>(&rule.Entries);
        entries->ResizeTo(static_cast<int32_t>(second.size()));
        for (const auto& condition : second) {
            entries->Add(condition);
        }
        rules->Add(rule);
        recipe->DefaultRecipeState = ERecipeState::Locked;
    }

    registeredRecipes.push_back(recipe);
    if (availableInLifePod)
        registeredRecipesLifePod.push_back(recipe);

    Log::Verbose("Recipe registered: {}", recipeId);
    return recipe;
}

std::vector<UUWECraftingRecipe*> RecipeFactory::getAllRegisteredRecipes() {
    return registeredRecipes;
}
