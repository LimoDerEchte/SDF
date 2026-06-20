//
// Created by Limo on 19/06/2026.
//

#pragma once

#include "registering/StoryGoalFactory.hpp"

class StoryGoalRuleParser {
    static std::unique_ptr<StoryGoalRule> parseSegment(const std::string& id, std::string& content, bool debug);

public:
    static std::unique_ptr<StoryGoalRule> parse(const std::string &id, std::string content, bool debug = false);
};
