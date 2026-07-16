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
    std::optional<UTexture2D*> icon;

    std::optional<UUWEStoryGoalRule*> unlockCondition = nullptr;
    std::optional<UUWEStoryGoal*> hideCondition = nullptr;

    bool modifyMode = false;
    bool categoriesModified = false;

    friend class Hooks;

public:
    explicit DatabankEntryFactory(std::string id, bool modifyMode);

    void setTitle(const std::string &newTitle);
    void setText(const std::string &newText);
    void addCategory(const std::string &category);

    bool setIcon(UTexture2D *newIcon);

    bool setUnlockCondition(const std::string &ruleId);
    bool setUnlockCondition(UUWEStoryGoalRule* goalRule);

    bool setHideCondition(const std::string &goalId);
    bool setHideCondition(UUWEStoryGoal* goal);

    UUWEDatabankEntry* registerDatabankEntry();
};
