//
// Created by Limo on 19/06/2026.
//

#include "StoryGoalRuleParser.hpp"

#include "util/Finders.hpp"
#include "util/Log.hpp"

using namespace SDK;

std::unique_ptr<StoryGoalRule> StoryGoalRuleParser::parseSegment(const std::string& id, std::string& content, const bool debug) {
    if (content.starts_with("'")) {
        const auto searchString = content.substr(1, content.find('\'', 1) - 1);
        content = content.substr(content.find('\'', 1) + 1);
        auto entry = std::make_unique<StoryGoalRequiredRule>();
        entry->goalId = id;
        entry->goal = Finders::searchStoryGoal(searchString);
        Log::Warning("Remaining content: {}", content);
        Log::Warning("Found story goal: {} --> {:p}", searchString, static_cast<void*>(entry->goal));
        return entry->goal == nullptr ? nullptr : std::move(entry);
    }
    if (content.starts_with("Negate(")) {
        auto entry = std::make_unique<StoryGoalRuleNegate>();
        auto rule = parseSegment(id, content, debug);
        if (rule == nullptr)
            return nullptr;
        entry->goalId = id;
        entry->rule = std::move(rule);
        content = content.substr(1);
        Log::Warning("Remaining content: {}", content);
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
                reinterpret_cast<StoryGoalRuleCount*>(entry.get())->count = std::stoi(content.substr(0, content.find(';')));
            } catch (std::exception& _) {
                Log::Warning("Failed to parse integer! Remaining content: {}", content);
                return nullptr;
            }
            content = content.substr(content.find(';') + 1);
        }
        entry->goalId = id;

        while (!content.empty()) {
            if (content.starts_with("]")) {
                content = content.substr(1);
                Log::Warning("Remaining content: {}", content);
                return entry;
            }
            if (content.starts_with(","))
                content = content.substr(1);
            auto rule = parseSegment(id, content, debug);
            if (rule == nullptr)
                return nullptr;
            entry->rules.push_back(std::move(rule));
        }
        Log::Warning("Invalid list! Remaining content: {}", content);
        return nullptr;
    }
    return nullptr;
}

std::unique_ptr<StoryGoalRule> StoryGoalRuleParser::parse(const std::string& id, std::string content, const bool debug) {
    std::erase_if(content, [](const unsigned char c) {
        return std::isspace(c);
    });
    return parseSegment(id, content, debug);
}
