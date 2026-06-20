//
// Created by Limo on 19/06/2026.
//

#include "StoryGoalRuleParser.hpp"

using namespace SDK;

std::unique_ptr<StoryGoalRule> StoryGoalRuleParser::parseSegment(const std::string& id, std::string& content) {
    if (content.starts_with("'")) {
        auto entry = std::make_unique<StoryGoalRequiredRule>();
        entry->goalId = id;
        entry->goal = Finders::searchStoryGoal(content.substr(1, content.find("'") - 1));
        return entry->goal == nullptr ? nullptr : std::move(entry);
    }
    if (content.starts_with("Negate(")) {
        auto entry = std::make_unique<StoryGoalRuleNegate>();
        auto rule = parseSegment(id, content);
        if (rule == nullptr)
            return nullptr;
        entry->goalId = id;
        entry->rule = std::move(rule);
        content = content.substr(1);
        return entry;
    }
    if (content.starts_with("And[") || content.starts_with("Or[") || content.starts_with("Count[")) {
        std::unique_ptr<StoryGoalRuleListType> entry;
        if (content.starts_with("And[")) {
            entry = std::make_unique<StoryGoalRuleAnd>();
            content = content.substr(4);
        } else if (content.starts_with("Or[")) {
            entry = std::make_unique<StoryGoalRuleOr>();
            content = content.substr(3);
        } else if (content.starts_with("Count[")) {
            entry = std::make_unique<StoryGoalRuleCount>();
            content = content.substr(6);
            try {
                reinterpret_cast<StoryGoalRuleCount*>(entry.get())->count = std::stoi(content.substr(content.find(";")));
            } catch (std::exception& _) {
                return nullptr;
            }
            content = content.substr(content.find(";") + 1);
        }
        entry->goalId = id;

        while (!content.empty()) {
            if (content.starts_with("]")) {
                content = content.substr(1);
                return entry;
            }
            if (content.starts_with(","))
                content = content.substr(1);
            auto rule = parseSegment(id, content);
            if (rule == nullptr)
                return nullptr;
            entry->rules.push_back(std::move(rule));
        }
        return nullptr;
    }
    return nullptr;
}

std::unique_ptr<StoryGoalRule> StoryGoalRuleParser::parse(const std::string& id, std::string content) {
    std::erase_if(content, [](const unsigned char c) {
        return std::isspace(c);
    });
    return parseSegment(id, content);
}
