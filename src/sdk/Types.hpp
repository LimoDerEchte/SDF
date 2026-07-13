//
// Created by limo on 7/8/26.
//

#pragma once

#include "Fixed.hpp"

using namespace RC;
using namespace Unreal;

class UTexture2D final : public UObject {
	// Stub Type
	ClassBase(Engine, Texture2D)
};

class UStaticMesh final : public UObject {
	// Stub Type
	ClassBase(Engine, StaticMesh)
};

class UMaterialInstance final : public UObject {
	// Stub Type
	ClassBase(Engine, MaterialInstance)
};

class UKismetRenderingLibrary final : public UObject {
	ClassBase(Engine, KismetRenderingLibrary)

	ClassFunctionRet2(ImportFileAsTexture2D, UTexture2D*, UObject*, WorldContextObject, const FString&, Filename)
};

class USN2AssetRegistry final : public UObject {
	// Stub Type
	ClassBase(Subnautica2, SN2AssetRegistry)
};

class UUWEPrimaryDataAssetBase : public UObject {
    ClassBase(UWEAssetRegistry, UWEPrimaryDataAssetBase)

    ClassProperty(PublishedStatus, EUWEPublishedStatus)
    ClassProperty(DeveloperNote, FString)
};

class UUWEScanData final : public UUWEPrimaryDataAssetBase
{
	// Stub Type
	ClassBase(UWEScanner, UWEScanData)
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
	ClassProperty(TriggeredDialogue, class UUWEDialogueStoryEvent*)
	ClassProperty(TriggeredNotification, class UUWENotificationStoryEvent*)
	ClassProperty(TriggeredTagStoryEvent, class UUWETagStoryEvent*)
	ClassProperty(StoryGroupTag, FGameplayTag)
	//ClassProperty(PlayerCurrencyRewards, (TMap<FGameplayTag, int32>)) TODO: Figure out maps

	ClassFunctionRet1(IsUnlockingRequirementsSatisfied, bool, AActor*, Actor);
};

class UUWEStoryEvent : public UUWEPrimaryDataAssetBase {
	ClassBase(UWEStoryEvents, UWEStoryEvent)

	ClassProperty(IdentifierTag, FGameplayTag)
};

class UUWEDialogueNode : public UUWEPrimaryDataAssetBase {
	// Missing Implementations
	ClassBase(UWEDialogue, UWEDialogueNode)
};

class UUWEDialogueStoryEvent final : public UUWEStoryEvent {
	ClassBase(UWEDialogue, UWEDialogueStoryEvent)

	ClassProperty(TriggeredDialogue, TSoftObjectPtr<UUWEDialogueNode>)
};

class UUWENotificationStoryEvent final : public UUWEStoryEvent {
	// Stub
	ClassBase(UWENotifications, UWENotificationStoryEvent)
};

class UUWETagStoryEvent final : public UUWEStoryEvent {
	ClassBase(UWEStoryEvents, UWETagStoryEvent)

	ClassProperty(EventTag, FGameplayTag)
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

class USN2BuilderActionData : public UUWEPrimaryDataAssetBase
{
	ClassBase(Subnautica2, SN2BuilderActionData)

	ClassProperty(Description, FText)
	ClassProperty(SecondaryDescription, FText)
	ClassProperty(PowerGenerationText, FText)
	ClassProperty(PowerDrainText, FText)
	ClassProperty(Thumbnail, TSoftObjectPtr<UTexture2D>)
	ClassProperty(Category, EBuilderMenuCategory)
	ClassProperty(RecipeCategory, TSoftObjectPtr<UUWECraftingRecipeCategory>)
	ClassProperty(OrderingIndex, int32)
	ClassProperty(EventTag, FGameplayTag)
	ClassProperty(ShowShowRecipeCategory, bool)
	ClassProperty(bShowRecipeCategory, bool)
	ClassProperty(Name_0, FText)
	ClassProperty(DefaultUnlockState, EUnlockState)
	ClassProperty(bNotifyOnUnlock, bool)
	ClassProperty(UpdatedUnlockingRequirements, TArray<FUWERecipeUnlockRules>)
	ClassProperty(TriggeredDialogue, UUWEDialogueStoryEvent*)
	ClassProperty(NotificationOverride, FText)
};

class USN2BuilderConstructActionData final : public USN2BuilderActionData
{
	ClassBase(Subnautica2, SN2BuilderConstructActionData)

	ClassProperty(GhostMeshOverride, TSoftObjectPtr<UStaticMesh>)
	ClassProperty(RenderGhostOverlaySeparately, bool)
	ClassProperty(UseGhostCameraOffsetOverride, bool)
	ClassProperty(GhostCameraOffsetOverride, float)
	ClassProperty(PlacementParams, FSN2PlacementParams)
	ClassProperty(TagsToAddDuringPlacement, FGameplayTagContainer)
	ClassProperty(Recipe, UUWECraftingRecipe*)
	ClassProperty(bDeconstructOnly, bool);
	ClassProperty(bSpawnAsDynamicItem, bool);
};

class UUWEItemType : public UUWEPrimaryDataAssetBase
{
	ClassBase(UWEInventory, UWEItemType)

	ClassProperty(TypeTag, FGameplayTag)
	ClassProperty(ActorClass, TSoftClassPtr<>)
	ClassProperty(HeldActorClass, TSoftClassPtr<>)
	ClassProperty(DefaultAttachment, UUWEItemType*)
	ClassProperty(Name_0, FText)
	ClassProperty(Thumbnail, TSoftObjectPtr<UTexture2D>)
	ClassProperty(ItemDescription, FText)
	ClassProperty(FabricationPreviewMaterialInstance, TSoftObjectPtr<UMaterialInstance>)
	ClassProperty(PickupDestination, EPickupDestination)
	ClassProperty(bReequipSameTypeOnRemove, bool)
	ClassProperty(EquipmentSlot, FGameplayTag)
	ClassProperty(AttachmentRules, FUWEEquipmentAttachmentRules)
	ClassProperty(Abilities, TArray<TSoftClassPtr<>>)
	ClassProperty(RequiredTagsToDrop, FGameplayTagContainer)
	ClassProperty(BlockingTagsToDrop, FGameplayTagContainer)
	ClassProperty(RequiredTagsToUse, FGameplayTagContainer)
	//ClassProperty(PlayerMissingTagWarnings, TMap<struct FGameplayTag, class FText>) TODO
	ClassProperty(RequiredUseTagsNotMetErrorText, FText)
	ClassProperty(CapturedAttributes, FGameplayTagContainer)
	ClassProperty(ConsumeEffects, TArray<TSoftClassPtr<>>)
	ClassProperty(GameplayTags, FGameplayTagContainer)
	//ClassProperty(TunableData, TMap<struct FGameplayTag, float>)
	ClassProperty(PickupCueTag, FGameplayTag)
	ClassProperty(DropCueTag, FGameplayTag)
	ClassProperty(EquipCue, FGameplayTag)
	ClassProperty(FirstEquipCue, FGameplayTag)
	ClassProperty(UnequipCue, FGameplayTag)
	ClassProperty(HolsterCue, FGameplayTag)
	ClassProperty(FailedActivationCue, FGameplayTag)
	ClassProperty(ItemAcquisitionStoryGoalsRef, TArray<UUWEStoryGoal*>)
	ClassProperty(EquippedStoryGoals, TArray<UUWEStoryGoal*>)
	//ClassProperty(ItemAnimation, FUWEItemAnimation*)
	ClassProperty(FabricationPreviewMeshData, FUWEFabricationPreviewMeshData*)
	ClassProperty(bEnergyTool, bool)
	ClassProperty(bShowNameWhenHovered, bool)
	ClassProperty(bUseFromInventory, bool)
	ClassProperty(bInvisibleInFirstPerson, bool)
	ClassProperty(bTool, bool)
	ClassProperty(bIsTwoHanded, bool)
	ClassProperty(bHideTunableDataInConsumableInfo, bool)
	ClassProperty(bNonSingleton, bool)
	ClassProperty(CarryableAttachmentTransform, FTransform)

public:
	TMap<struct FGameplayTag, class FString> GetDefaultCapturedAttributes();
	class UTexture2D* GetThumbnailFromAttributeMap(const TMap<struct FGameplayTag, class FString>& Map);

	class UClass* GetHeldActorClassLoaded() const;
	class UClass* GetItemAnimEquippedAnimBlueprintClass() const;
	struct FUWEFabricationPreviewMeshData GetPreviewMeshData() const;
	void GetTunableData(const struct FGameplayTag& Tag, float* Value, bool* bFound) const;
	bool IsEquippable() const;
	bool IsEquippment() const;
	void PreloadItemAnimBlueprintClass() const;
};
