//
// Created by limo on 7/10/26.
//

#pragma once

#include "CommonSDK.hpp"

using namespace RC;
using namespace Unreal;

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

struct FGameplayTag
{
    FName TagName;
};

struct FStoryGoalUnlock final
{
    class UUWEStoryGoal* GoalToUnlockRef;
    float DelayTime;
    uint8 Pad_C[0x4];
};

struct FCraftingRecipeOutput final
{
    TSoftObjectPtr<class UUWEItemType> ItemType;
    int32 NumItems;
    uint8 Pad_2C[0x4];
    FVector CollisionCheckSize;
    TArray<TSoftClassPtr<UClass>> ClassesToIgnoreForCollisionCheck;
};

struct FScheduledStoryGoal final
{
    UUWEStoryGoal* GoalToUnlock;
    float DelayTime;
    uint8 Pad_C[0x4];
};
