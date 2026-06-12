//
// Created by Limo on 21/05/2026.
//

#include "registering/RecipeFactory.hpp"

#include "util/Log.hpp"
#include "UObjectGlobals.hpp"
#include "UObject.hpp"
#include "util/Finders.hpp"

using namespace SDK;
using namespace RC;
using namespace Unreal;

using EF = SDK::EObjectFlags;

std::vector<UUWECraftingRecipe*> RecipeFactory::registeredRecipes;
std::vector<UUWECraftingRecipe*> RecipeFactory::registeredRecipesLifePod;

void RecipeFactory::unregisterAllRecipes() {
    registeredRecipesLifePod.clear();
    for (const auto& registered_recipe : registeredRecipes)
        reinterpret_cast<Unreal::UObject*>(registered_recipe)->BeginDestroy();
    registeredRecipes.clear();
}

RecipeFactory::RecipeFactory(std::string recipeId, const bool isModify)
    : recipeId(std::move(recipeId)), recipeName("Empty"), recipeDescription("Empty"), recipeTexture() {
    modifyMode = isModify;
    if (isModify) {
        craftingTime = -1;
    } else {
        const auto defaultTex = reinterpret_cast<UTexture2D*>(UObjectGlobals::FindObject(L"Texture2D", L"T_DefaultImage"));
        setIcon(defaultTex);
    }
}

void RecipeFactory::setName(const std::string &recipeName) {
    this->recipeName = recipeName;
}

void RecipeFactory::setDescription(const std::string &recipeDescription) {
    this->recipeDescription = recipeDescription;
}

bool RecipeFactory::setCategory(const std::string &categoryId) {
    return setCategory(Finders::searchRecipeCategory(categoryId));
}

bool RecipeFactory::setCategory(UUWECraftingRecipeCategory *category) {
    if (category == nullptr)
        return false;
    recipeCategory = category;
    return true;
}

bool RecipeFactory::setIconFromItem(const std::string &itemId) {
    return setIconFromItem(Finders::searchItem(itemId));
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
    recipeTextureModified = true;
    recipeTexture = static_cast<TSoftObjectPtr<UTexture2D>>(UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(icon));
    return true;
}

void RecipeFactory::setIcon(const TSoftObjectPtr<UTexture2D> &icon) {
    recipeTextureModified = true;
    recipeTexture = icon;
}

bool RecipeFactory::addIngredient(const std::string &itemId, const int32_t amount) {
    return addIngredient(Finders::searchItem(itemId), amount);
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
    return addOutput(Finders::searchItem(itemId), amount);
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
    return addUnlockingRequirementPickup(ruleSet, Finders::searchItem(itemId));
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
    return addUnlockingRequirementScanData(ruleSet, Finders::searchScanData(dataId));
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

void RecipeFactory::modifyRemoveRequirements() {
    removeRequirementsModify = true;
}

void RecipeFactory::setCraftingTime(const float time) {
    craftingTime = time;
}

void RecipeFactory::setAvailableInLifepod(const bool available) {
    availableInLifePod = available;
    availableInLifePodModify = true;
}

void RecipeFactory::setOrderingIndex(const int orderingIndex) {
    this->orderingIndex = orderingIndex;
    orderingIndexModify = true;
}

UUWECraftingRecipe* RecipeFactory::registerRecipe() const {
    const auto base = Finders::searchRecipe("MetalSalvage");
    if (base == nullptr)
        return nullptr;

    const auto recipe = modifyMode ? Finders::searchRecipe(recipeId) : static_cast<UUWECraftingRecipe*>(UGameplayStatics::SpawnObject(UUWECraftingRecipe::StaticClass(), base->Outer));
    if (recipe == nullptr)
        return nullptr;

    if (!modifyMode) {
        recipe->Name = UKismetStringLibrary::Conv_StringToName(UtfN::StringToWString(std::format("DA_{}Recipe", recipeId)).c_str());
        recipe->Flags = EF::MarkAsRootSet | EF::Public | EF::Standalone | EF::Transactional | EF::WasLoaded | EF::LoadCompleted;
    }
    if (orderingIndexModify)
        recipe->OrderingIndex = orderingIndex;

    if (recipeTextureModified)
        recipe->Thumbnail = recipeTexture;
    if (!modifyMode || recipeName != "Empty")
        recipe->Name_0 = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(recipeName).c_str());
    if (!modifyMode || recipeDescription != "Empty")
        recipe->Description = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(recipeDescription).c_str());

    if (!modifyMode || recipeCategory != nullptr)
        recipe->Category = recipeCategory == nullptr ? base->Category : static_cast<TSoftObjectPtr<UUWECraftingRecipeCategory>>(UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(recipeCategory));
    if (!modifyMode || craftingTime != -1)
        recipe->CraftingTime = craftingTime;

    const auto requirements = reinterpret_cast<Unreal::TArray<FCraftingRecipeRequirement>*>(&recipe->Requirements);
    if (modifyMode && !ingredients.empty())
        requirements->SetNum(0, EAllowShrinking::Yes);

    requirements->ResizeTo(requirements->Num() + static_cast<int32_t>(ingredients.size()));
    for (const auto& ingredient: ingredients) {
        requirements->Add(ingredient);
    }

    const auto output = reinterpret_cast<Unreal::TArray<FCraftingRecipeOutput>*>(&recipe->Output);
    if (modifyMode && !outputs.empty())
        output->SetNum(0, EAllowShrinking::Yes);

    output->ResizeTo(output->Num() + static_cast<int32_t>(outputs.size()));
    for (const auto& out : outputs) {
        output->Add(out);
    }

    const auto rules = reinterpret_cast<Unreal::TArray<FUWERecipeUnlockRules>*>(&recipe->UpdatedUnlockingRequirements);
    if (modifyMode && (!unlockingRules.empty() ||removeRequirementsModify)) {
        rules->SetNum(0, EAllowShrinking::Yes);
        recipe->DefaultRecipeState = ERecipeState::Unlocked;
    }

    rules->ResizeTo(rules->Num() + static_cast<int32_t>(unlockingRules.size()));
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
    if (availableInLifePodModify) {
        const auto staticBrokenFabricator = UObjectGlobals::StaticFindObject(nullptr, nullptr, L"/Game/Blueprints/Crafting/BP_Fabricator_Lifepod.Default__BP_Fabricator_Lifepod_C:Crafter");
        const auto lifepodCrafter = reinterpret_cast<UUWECrafterComponent*>(staticBrokenFabricator);

        if (lifepodCrafter == nullptr) {
            Log::Warning("Failed to find lifepod fabricator component");
            return recipe;
        }
        const auto itemList = reinterpret_cast<Unreal::TArray<TSoftObjectPtr<SDK::UObject>>*>(&lifepodCrafter->AllowedRecipesOverride);

        if (availableInLifePod) {
            itemList->Add(UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(recipe));
            registeredRecipesLifePod.push_back(recipe);
        } else if (modifyMode) {
            for (int i = 0; i < itemList->Num(); i++) {
                if ((*itemList)[i].ObjectID.AssetPath.AssetName.ToString() != recipe->GetName())
                    continue;
                itemList->RemoveAt(i);
                break;
            }
        }
    }

    Log::Verbose("Recipe {}: {}", modifyMode ? "modified" : "registered", recipeId);
    return recipe;
}
