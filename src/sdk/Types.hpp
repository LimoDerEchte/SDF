//
// Created by limo on 7/8/26.
//

#pragma once

#include "Fixed.hpp"

using namespace RC;
using namespace Unreal;

class UTexture2D;
class UUWEPrimaryDataAssetBase;
class UUWEStoryGoal;
class UUWEDatabankEntry;

class UTexture2D final : public UObject {
	ClassBase(Engine, Texture2D)
};

class UUWEPrimaryDataAssetBase : public UObject {
    ClassBase(UWEAssetRegistry, UWEPrimaryDataAssetBase)

    ClassProperty(PublishedStatus, EUWEPublishedStatus)
    ClassProperty(DeveloperNote, FString)
};

class UUWEStoryGoal final : public UUWEPrimaryDataAssetBase {
	ClassBase(UWEStoryGoals, UWEStoryGoal)

	ClassProperty(StoryGoalType, EStoryGoalTargetType)
	ClassProperty(StoryGoalUnlocks, TArray<FStoryGoalUnlock>)
	ClassProperty(UnlockingRequirements, UUWEStoryGoalRule*)
	ClassProperty(UnlockOnRequirementsMet, bool)
	ClassProperty(TriggeredDialogue, UUWEDialogueStoryEvent*)
	ClassProperty(TriggeredNotification, UUWENotificationStoryEvent*)
	ClassProperty(TriggeredTagStoryEvent, UUWETagStoryEvent*)
	ClassProperty(StoryGroupTag, FGameplayTag)
	//ClassProperty(PlayerCurrencyRewards, (TMap<FGameplayTag, int32>)) TODO: Figure out maps

	ClassFunctionRet1(IsUnlockingRequirementsSatisfied, bool, AActor*, Actor);
};

class UUWEDatabankEntry : public UUWEPrimaryDataAssetBase {
	ClassBase(UWEDatabank, UWEDatabankEntry)

	ClassProperty(Categories, TArray<FText>)
	ClassProperty(EntryTitle, FText);
	ClassProperty(EntryText, FText);
	ClassProperty(EntryImage, TSoftObjectPtr<UTexture2D>);
	ClassProperty(UnlockingRequirements, UUWEStoryGoalRule*);
	ClassProperty(HideOnStoryGoal, UUWEStoryGoal*);
	ClassProperty(bCompletesInvestigation, bool);

	ClassFunctionRet0(IsUnlocked, bool);
};

class UUWECraftingRecipe final : public UUWEPrimaryDataAssetBase {
	ClassBase(UWECrafting, UWECraftingRecipe)

	ClassProperty(Name_0, FText)
	ClassProperty(Description, FText)
	ClassProperty(Thumbnail, TSoftObjectPtr<UTexture2D>)
	ClassProperty(TriggeredDialogue, UUWEDialogueStoryEvent*)
	ClassProperty(NotificationOverride, FText)
	ClassProperty(Output, TArray<FCraftingRecipeOutput>)
	ClassProperty(Requirements, TArray<FCraftingRecipeRequirement>)
	ClassProperty(DefaultRecipeState, ERecipeState)
	ClassProperty(bNotifyOnUnlock, bool)
	ClassProperty(UpdatedUnlockingRequirements, TArray<FUWERecipeUnlockRules>)
	ClassProperty(CraftingTime, float)
	ClassProperty(Category, TSoftObjectPtr<UUWECraftingRecipeCategory>)
	ClassProperty(OrderingIndex, int32)
	ClassProperty(DuplicatesBuilderActionData, bool)
};
