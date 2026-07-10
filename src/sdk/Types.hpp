//
// Created by limo on 7/8/26.
//

#pragma once

#include "Fixed.hpp"

using namespace RC;
using namespace Unreal;

class UTexture2D final : public UObject {
	ClassBase(Engine, Texture2D)
};

class UUWEPrimaryDataAssetBase : public UObject {
    ClassBase(UWEAssetRegistry, UWEPrimaryDataAssetBase)

    ClassProperty(PublishedStatus, EUWEPublishedStatus)
    ClassProperty(DeveloperNote, FString)
};

class UUWEStoryGoalRule : public UObject {
	ClassBase(UWEStoryGoals, UWEStoryGoalRule)

	ClassFunctionRet1(IsRuleSatisfied, bool, AActor*, Actor)
};

class UUWERequiredStoryGoalRule final : public UUWEStoryGoalRule
{
	ClassBase(UWEStoryGoals, UWERequiredStoryGoalRule)

	ClassProperty(RequiredStoryGoalRef, UUWEStoryGoal*)
};

class UUWEEventTrackerWorldCountRule final : public UUWEStoryGoalRule
{
	ClassBase(UWEStoryGoals, UWEEventTrackerWorldCountRule)

	ClassProperty(EventVerb, FGameplayTag)
	ClassProperty(EventTag, FGameplayTag)
	ClassProperty(EventAsset, UUWEPrimaryDataAssetBase*)
	ClassProperty(MinimumCount, int32)
};

class UUWEEventTrackerPlayerCountRule final : public UUWEStoryGoalRule
{
	ClassBase(UWEStoryGoals, UWEEventTrackerPlayerCountRule)

	ClassProperty(EventVerb, FGameplayTag)
	ClassProperty(EventTag, FGameplayTag)
	ClassProperty(EventAsset, UUWEPrimaryDataAssetBase*)
	ClassProperty(MinimumCount, int32)
};

class UUWEStoryGoalRuleAnd final : public UUWEStoryGoalRule
{
	ClassBase(UWEStoryGoals, UWEStoryGoalRuleAnd)

	ClassProperty(Rules, TArray<UUWEStoryGoalRule*>)
};

class UUWEStoryGoalRuleOr final : public UUWEStoryGoalRule
{
	ClassBase(UWEStoryGoals, UWEStoryGoalRuleOr)

	ClassProperty(Rules, TArray<UUWEStoryGoalRule*>)
};

class UUWEStoryGoalRuleNegate final : public UUWEStoryGoalRule
{
	ClassBase(UWEStoryGoals, UWEStoryGoalRuleNegate)

	ClassProperty(RuleToNegate, UUWEStoryGoalRule*)
};

class UUWEStoryGoalRuleCount final : public UUWEStoryGoalRule
{
	ClassBase(UWEStoryGoals, UWEStoryGoalRuleCount)

	ClassProperty(Rules, TArray<UUWEStoryGoalRule*>)
	ClassProperty(MinimumCount, int32)
};

class UUWEStoryGoalSchedule final : public UUWEPrimaryDataAssetBase
{
	ClassBase(UWEStoryGoals, UWEStoryGoalSchedule)

	ClassProperty(ScheduledStoryGoals, TArray<FScheduledStoryGoal>)
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

class UUWECraftingRecipeCategory final : public UUWEPrimaryDataAssetBase
{
	ClassBase(UWECrafting, UWECraftingRecipeCategory)

	ClassProperty(Name_0, FText)
	ClassProperty(Description, FText)
	ClassProperty(CraftedBy, ECrafterType)
	ClassProperty(Thumbnail, TSoftObjectPtr<UTexture2D>)
	ClassProperty(OrderingIndex, int32)
	ClassProperty(ParentCategory, TSoftObjectPtr<UUWECraftingRecipeCategory>)
	ClassProperty(bShowWhenEmpty, bool)

	ClassFunctionRet0(GetCrafterText, FText)
};
