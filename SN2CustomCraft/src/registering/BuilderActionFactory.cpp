//
// Created by Limo on 25/05/2026.
//

#include "BuilderActionFactory.hpp"

#include "RecipeFactory.hpp"
#include "UObjectGlobals.hpp"
#include "util/Finders.hpp"
#include "util/Log.hpp"
#include "util/RegistryHelper.hpp"

using namespace SDK;
using namespace RC;
using namespace Unreal;

using EF = SDK::EObjectFlags;

std::vector<USN2BuilderActionData*> BuilderActionFactory::registeredActions;

BuilderActionFactory::BuilderActionFactory(std::string recipeId)
    : recipeId(std::move(recipeId)) {
}

void BuilderActionFactory::addPowerDrainText(std::string text) {
    powerDrainText = text;
}

void BuilderActionFactory::addPowerGenerationText(std::string text) {
    powerGenerationText = text;
}

USN2BuilderConstructActionData* BuilderActionFactory::registerBuilderAction() const {
    const auto base = static_cast<USN2BuilderConstructActionData*>(Finders::searchBuilderAction("Table_DiningConstructData"));
    if (base == nullptr)
        return nullptr;

    const auto recipe = Finders::searchRecipe(recipeId);
    if (recipe == nullptr) {
        Log::Warning("Couldn't find reference recipe {}", recipeId);
        return nullptr;
    }

    const auto action = static_cast<USN2BuilderConstructActionData*>(UGameplayStatics::SpawnObject(USN2BuilderConstructActionData::StaticClass(), base->Outer));
    if (recipe == nullptr)
        return nullptr;

    action->Name = UKismetStringLibrary::Conv_StringToName(UtfN::StringToWString(std::format("DA_{}_ConstructData", recipeId)).c_str());
    action->Flags = EF::MarkAsRootSet | EF::Public | EF::Standalone | EF::Transactional | EF::WasLoaded | EF::LoadCompleted;

    action->Name_0 = recipe->Name_0;
    action->Description = recipe->Description;
    action->SecondaryDescription = UKismetTextLibrary::Conv_StringToText(L"WIP"); // TODO: Either automatic or provide func
    action->Thumbnail = recipe->Thumbnail;

    if (powerDrainText.has_value())
        action->PowerDrainText = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(powerDrainText.value()).c_str());

    if (powerGenerationText.has_value())
        action->PowerGenerationText = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(powerGenerationText.value()).c_str());

    action->Category = base->Category;
    action->EventTag = base->EventTag;

    action->DefaultUnlockState = recipe->DefaultRecipeState == ERecipeState::Unlocked ? EUnlockState::Unlocked : EUnlockState::Locked;
    action->UpdatedUnlockingRequirements = recipe->UpdatedUnlockingRequirements;
    action->RecipeCategory = recipe->Category;
    action->Recipe = recipe;

    // TODO: All after this should be configurable

    action->GhostMeshOverride = base->GhostMeshOverride;
    action->PlacementParams = base->PlacementParams;
    action->bSpawnAsDynamicItem = true;

    registeredActions.push_back(action);
    RegistryHelper::AddToRegistry(action, "SN2BuilderConstructActionData");

    Log::Verbose("Builder action registered: {}", recipeId);
    return action;
}
