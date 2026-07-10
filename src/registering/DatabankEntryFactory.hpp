//
// Created by Limo on 13/06/2026.
//

#pragma once

#include "sdk/Types.hpp"

class DatabankEntryFactory {
    static std::vector<UUWEDatabankEntry*> registeredDatabankEntries;

    std::string id;
    std::optional<std::string> title;
    std::optional<std::string> text;

    std::vector<std::string> categories;
    TSoftObjectPtr<UTexture2D> icon;

    UUWEStoryGoalRule* unlockCondition = nullptr;
    UUWEStoryGoal* hideCondition = nullptr;

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

    bool setIcon(UTexture2D *newIcon);
    void setIcon(const TSoftObjectPtr<UTexture2D> &newIcon);

    bool setUnlockCondition(const std::string &ruleId);
    bool setUnlockCondition(UUWEStoryGoalRule* goalRule);

    bool setHideCondition(const std::string &goalId);
    bool setHideCondition(UUWEStoryGoal* goal);

    UUWEDatabankEntry* registerDatabankEntry();
};
