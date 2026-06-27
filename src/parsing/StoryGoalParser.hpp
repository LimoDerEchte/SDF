//
// Created by Limo on 19/06/2026.
//

#pragma once

#include <map>

#include "registering/StoryGoalRuleFactory.hpp"
#include "SDK/UWECrafting_classes.hpp"
#include "toml++/impl/table.hpp"

class StoryGoalParser {
    static std::map<std::string, SDK::UUWEStoryGoal*> storyGoals;

    static void parseFile(const std::string &file, const toml::table &table);

public:
    static void ParseStoryGoals();
};
