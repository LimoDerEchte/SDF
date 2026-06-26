//
// Created by Limo on 25/05/2026.
//

#include "registering/BuilderActionFactory.hpp"

#include "registering/RecipeFactory.hpp"
#include "UObjectGlobals.hpp"
#include "util/Finders.hpp"
#include "util/Log.hpp"
#include "util/RegistryHelper.hpp"

using namespace SDK;
using namespace RC;
using namespace Unreal;

std::vector<USN2BuilderActionData*> BuilderActionFactory::registeredActions;

BuilderActionFactory::BuilderActionFactory(std::string id, const bool modifyMode)
    : id(std::move(id)), recipe(nullptr), modifyMode(modifyMode) {
}

void BuilderActionFactory::setRemovePowerText(const bool removePowerText) {
    this->removePowerText = removePowerText;
}

void BuilderActionFactory::setSecondaryDescription(const std::string &secondaryDescription) {
    this->secondaryDescription = secondaryDescription;
    modifySecondaryDescription = true;
}

void BuilderActionFactory::addPowerDrainText(const std::string& text) {
    powerDrainText = text;
}

void BuilderActionFactory::addPowerGenerationText(const std::string& text) {
    powerGenerationText = text;
}

bool BuilderActionFactory::setRecipe(const std::string &recipe) {
    return setRecipe(Finders::searchRecipe(recipe));
}

bool BuilderActionFactory::setRecipe(UUWECraftingRecipe *recipe) {
    if (recipe == nullptr)
        return false;
    this->recipe = recipe;
    recipeModified = true;
    return true;
}

USN2BuilderConstructActionData* BuilderActionFactory::registerBuilderAction() const {
    const auto base = Finders::searchFirstOf<USN2BuilderConstructActionData>("SN2BuilderConstructActionData");
    if (base == nullptr)
        return nullptr;

    const auto action = modifyMode ? Finders::searchBuilderAction(id) : RegistryHelper::StaticConstructTemplate(base, std::format("DA_{}_ConstructData", id));
    if (action == nullptr)
        return nullptr;

    if (recipe != nullptr) {
        action->Name_0 = recipe->Name_0;
        action->Description = recipe->Description;
        action->Thumbnail = recipe->Thumbnail;

        action->RecipeCategory = recipe->Category;
        action->Recipe = recipe;

        action->DefaultUnlockState = recipe->DefaultRecipeState == ERecipeState::Unlocked ? EUnlockState::Unlocked : EUnlockState::Locked;
        action->UpdatedUnlockingRequirements = recipe->UpdatedUnlockingRequirements;
    }

    if (modifySecondaryDescription)
        action->SecondaryDescription = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(secondaryDescription).c_str());
    else if (!modifyMode)
        action->SecondaryDescription = UKismetTextLibrary::Conv_StringToText(L"Empty");

    if (removePowerText) {
        action->PowerDrainText = UKismetTextLibrary::Conv_StringToText(L"");
        action->PowerGenerationText = UKismetTextLibrary::Conv_StringToText(L"");
    }
    else if (powerDrainText.has_value())
        action->PowerDrainText = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(powerDrainText.value()).c_str());
    else if (powerGenerationText.has_value())
        action->PowerGenerationText = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(powerGenerationText.value()).c_str());

    action->Category = base->Category;
    action->EventTag = base->EventTag;

    // TODO: All after this should be configurable

    action->GhostMeshOverride = base->GhostMeshOverride;
    action->PlacementParams = base->PlacementParams;
    action->bSpawnAsDynamicItem = true;

    registeredActions.push_back(action);
    RegistryHelper::AddToRegistry(action, "SN2BuilderConstructActionData");

    Log::Verbose("Builder action registered: {}", id);
    return action;
}
