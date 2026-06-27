//
// Created by Limo on 27/06/2026.
//

#pragma once

#include "registering/StoryGoalRuleFactory.hpp"

class StoryGoalRuleParser {
    static std::unique_ptr<StoryGoalRule> parseRuleSegment(const std::string& id, std::string& content, bool debug);

public:
    static std::unique_ptr<StoryGoalRule> ParseRule(const std::string &id, std::string content, bool debug = false);
};
