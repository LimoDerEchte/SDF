//
// Created by limo on 7/10/26.
//

#pragma once

#include "CommonSDK.hpp"

using namespace RC;
using namespace Unreal;

enum class EPickupDestination : uint8
{
	Inventory                                = 0,
	Unused                                   = 1,
	Equipment                                = 2,
	CannotPickup                             = 3,
	Toolbar                                  = 10,
	ToolbarAndEquip                          = 11,
	ToolbarAndAlwaysEquip                    = 12,
	ToolbarAndAlwaysEquipIfNew               = 13,
	EPickupDestination_MAX                   = 14,
};

enum class EUWEPublishedStatus : uint8
{
    Published                                = 0,
    Unpublished                              = 1,
    Deprecated                               = 2,
    EUWEPublishedStatus_MAX                  = 3,
};

enum class EStoryGoalTargetType : uint8
{
    NotSet                                   = 0,
    World                                    = 1,
    Player                                   = 2,
    Databank                                 = 3,
    Invalid                                  = 4,
    EStoryGoalTargetType_MAX                 = 5,
};

enum class ECrafterType : uint8
{
    Undefined                                = 0,
    Fabricator                               = 1,
    BuilderTool                              = 2,
    Refinery                                 = 3,
    VehicleFabricator                        = 4,
    ModificationStation                      = 5,
    ECrafterType_MAX                         = 6,
};

enum class EBuilderMenuCategory : uint8
{
    Sculpt                                   = 0,
    BaseObject                               = 1,
    WorldObject                              = 2,
    Interior                                 = 3,
    PredefinedLocationOnly                   = 4,
    Count                                    = 5,
    EBuilderMenuCategory_MAX                 = 6,
};

enum class EUnlockState : uint8
{
    Locked                                   = 0,
    Unlocked                                 = 1,
    EUnlockState_MAX                         = 2,
};

enum class EBuilderSnapping : uint8
{
	Freeform                                 = 0,
	Base                                     = 1,
	None                                     = 2,
	EBuilderSnapping_MAX                     = 3,
};

enum class ERecipeState : uint8
{
	Locked                                   = 0,
	Unlocked                                 = 1,
	ERecipeState_MAX                         = 2,
};

struct FGameplayTag
{
    FName TagName;
};

struct FGameplayTagContainer final
{
	TArray<FGameplayTag> GameplayTags;
	TArray<FGameplayTag> ParentTags;
};

struct FStoryGoalUnlock final
{
    class UUWEStoryGoal* GoalToUnlockRef;
    float DelayTime;
    uint8 Pad_C[0x4];
};

struct FUWEEquipmentAttachmentRules final
{
	bool bSetParentAsLeaderPose;
	bool bUseExplicitAttachmentSocketName;
	uint8 Pad_2[0x2];
	FName AttachmentSocketName;
};

struct FCraftingRecipeOutput final
{
    TSoftObjectPtr<class UUWEItemType> ItemType;
    int32 NumItems;
    uint8 Pad_2C[0x4];
    FVector CollisionCheckSize;
    TArray<TSoftClassPtr<UClass>> ClassesToIgnoreForCollisionCheck;
};

struct FUWEPrimaryDataAssetBaseArray final
{
	TArray<class UUWEPrimaryDataAssetBase*> Assets;
};

struct FScheduledStoryGoal final
{
    UUWEStoryGoal* GoalToUnlock;
    float DelayTime;
    uint8 Pad_C[0x4];
};

struct FUWERecipeUnlockRules final
{
	FText RuleName;
	TArray<struct FUWERecipeUnlockRuleEntry> Entries;
};

struct FCraftingRecipeRequirement final
{
	TSoftObjectPtr<UUWEItemType> ItemType;
	int32 NumItems;
	uint8 Pad_2C[0x4];
};

struct FSN2PlacementParams final
{
	int32 AllowedSurfaces;
	int32 AllowedZones;
	FGameplayTagContainer AllowedBrushTypes;
	FGameplayTagContainer AllowedSurfaceTags;
	bool OverrideCurrentSnapping;
	EBuilderSnapping SnappingOverride;
	bool OverrideLocationSnapSize;
	uint8 Pad_4B[0x1];
	float LocationSnapSize;
	bool OverrideUserRotationIncrement;
	uint8 Pad_51[0x3];
	float UserRotationIncrement;
	TArray<struct FSN2SnappingSocket> SnappingSockets;
	TArray<class UUWEBaseModule*> ProhibitedModules;
	int32 RotatesAtPlacementSurface;
	bool bUseSnappingRotation;
	bool bOverrideInteractDistance;
	bool bAllowUserRotationOnWall;
	uint8 Pad_7F[0x1];
	float InteractDistance;
	float HorizontalSurfaceThreshold;
};
