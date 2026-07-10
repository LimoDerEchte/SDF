//
// Created by limo on 7/10/26.
//

#pragma once

#include "CommonSDK.hpp"

using namespace RC;
using namespace Unreal;

enum class EUWEPublishedStatus;
enum class EStoryGoalTargetType;
struct FGameplayTag;
struct FStoryGoalUnlock;

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
