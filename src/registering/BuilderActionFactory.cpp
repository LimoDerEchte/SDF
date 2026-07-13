//
// Created by Limo on 25/05/2026.
//

#include "registering/BuilderActionFactory.hpp"

#include "FText.hpp"
#include "registering/RecipeFactory.hpp"
#include "UObjectGlobals.hpp"
#include "util/Finders.hpp"
#include "util/Log.hpp"
#include "util/RegistryHelper.hpp"

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
        action->SetName_0(*recipe->GetName_0());
        action->SetDescription(*recipe->GetDescription());
        action->SetThumbnail(*recipe->GetThumbnail());

        action->SetRecipeCategory(*recipe->GetCategory());
        action->SetRecipe(recipe);

        action->SetDefaultUnlockState(*recipe->GetDefaultRecipeState() == ERecipeState::Unlocked ? EUnlockState::Unlocked : EUnlockState::Locked);
        action->SetUpdatedUnlockingRequirements(*recipe->GetUpdatedUnlockingRequirements());
    }

    if (modifyMode || secondaryDescription.has_value())
        action->SetSecondaryDescription(FText(UtfN::StringToWString(secondaryDescription.value_or("Empty")).c_str()));

    if (removePowerText) {
        action->SetPowerDrainText(FText(L""));
        action->SetPowerGenerationText(FText(L""));
    }
    else if (powerDrainText.has_value())
        action->SetPowerDrainText(FText(UtfN::StringToWString(powerDrainText.value()).c_str()));
    else if (powerGenerationText.has_value())
        action->SetPowerGenerationText(FText(UtfN::StringToWString(powerGenerationText.value()).c_str()));

    action->SetCategory(*base->GetCategory());
    action->SetEventTag(*base->GetEventTag());

    // TODO: All after this should be configurable

    action->SetGhostMeshOverride(*base->GetGhostMeshOverride());
    action->SetPlacementParams(*base->GetPlacementParams());
    action->SetbSpawnAsDynamicItem(true);

    registeredActions.push_back(action);
    RegistryHelper::AddToRegistry(action, "SN2BuilderConstructActionData");

    Log::Verbose("Builder action registered: {}", id);
    return action;
}
