//
// Created by Limo on 13/06/2026.
//

#pragma once

#include "UnrealDef.hpp"
#include "UnrealCoreStructs.hpp"
#include "SDK/UWEDatabank_classes.hpp"

class DatabankEntryFactory {
    static std::vector<SDK::UUWEDatabankEntry*> registeredDatabankEntries;

    std::string id;
    std::optional<std::string> title;
    std::optional<std::string> text;

    std::vector<std::string> categories;
    SDK::TSoftObjectPtr<SDK::UTexture2D> icon;

    SDK::UUWEStoryGoalRule* unlockCondition = nullptr;
    SDK::UUWEStoryGoal* hideCondition = nullptr;

    bool modifyMode = false;
    bool iconModified = false;
    bool categoriesModified = false;
    bool unlockConditionModified = false;
    bool hideConditionModified = false;

    friend class Hooks;

public:
    explicit DatabankEntryFactory(const std::string &id, bool modifyMode);

    void setTitle(const std::string &newTitle);
    void setText(const std::string &newText);
    void addCategory(const std::string &category);

    bool setIcon(SDK::UTexture2D *newIcon);
    void setIcon(const SDK::TSoftObjectPtr<SDK::UTexture2D> &newIcon);

    bool setUnlockCondition(const std::string &ruleId);
    bool setUnlockCondition(SDK::UUWEStoryGoalRule* goalRule);

    bool setHideCondition(const std::string &goalId);
    bool setHideCondition(SDK::UUWEStoryGoal* goal);

    SDK::UUWEDatabankEntry* registerDatabankEntry();
};
