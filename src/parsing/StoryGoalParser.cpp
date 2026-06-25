//
// Created by Limo on 19/06/2026.
//

#include "StoryGoalParser.hpp"

#include "FileTraversal.hpp"
#include "util/Finders.hpp"
#include "util/Log.hpp"

using namespace SDK;

std::map<std::string, UUWEStoryGoal*> StoryGoalParser::storyGoals{};

const std::map<std::string, EStoryGoalTargetType> storyGoalTypeMap = {
    { "Databank", EStoryGoalTargetType::Databank },
    { "Player", EStoryGoalTargetType::Player },
    { "World", EStoryGoalTargetType::World }
};

std::unique_ptr<StoryGoalRule> StoryGoalParser::parseRuleSegment(const std::string& id, std::string& content, const bool debug) {
    if (content.starts_with("'")) {
        const auto searchString = content.substr(1, content.find('\'', 1) - 1);
        content = content.substr(content.find('\'', 1) + 1);
        auto entry = std::make_unique<StoryGoalRequiredRule>();
        entry->goalId = id;
        entry->goal = Finders::searchStoryGoal(searchString);
        if (debug) {
            Log::Warning("Remaining content: {}", content);
            Log::Warning("Found story goal: {} --> {:p}", searchString, static_cast<void*>(entry->goal));
        }
        return entry->goal == nullptr ? nullptr : std::move(entry);
    }
    if (content.starts_with("Negate(")) {
        auto entry = std::make_unique<StoryGoalRuleNegate>();
        auto rule = parseRuleSegment(id, content, debug);
        if (rule == nullptr)
            return nullptr;
        entry->goalId = id;
        entry->rule = std::move(rule);
        content = content.substr(1);
        if (debug) Log::Warning("Remaining content: {}", content);
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
                if (debug) Log::Warning("Failed to parse integer! Remaining content: {}", content);
                return nullptr;
            }
            content = content.substr(content.find(';') + 1);
        }
        entry->goalId = id;

        while (!content.empty()) {
            if (content.starts_with("]")) {
                content = content.substr(1);
                if (debug) Log::Warning("Remaining content: {}", content);
                return entry;
            }
            if (content.starts_with(","))
                content = content.substr(1);
            auto rule = parseRuleSegment(id, content, debug);
            if (rule == nullptr)
                return nullptr;
            entry->rules.push_back(std::move(rule));
        }
        if (debug) Log::Warning("Invalid list! Remaining content: {}", content);
        return nullptr;
    }
    return nullptr;
}

std::unique_ptr<StoryGoalRule> StoryGoalParser::ParseRule(const std::string& id, std::string content, const bool debug) {
    std::erase_if(content, [](const unsigned char c) {
        return std::isspace(c);
    });
    return parseRuleSegment(id, content, debug);
}

void StoryGoalParser::parseFile(const std::string &file, const toml::table &table) {
    if (!table.contains("id") || !table["id"].is_string()) {
        Log::Warning("File {} has a story goal without an id", file);
        return;
    }
    const auto id = table["id"].as_string()->get();

    auto factory = StoryGoalFactory(id);

    if (table.contains("type")) {
        if (!table["type"].is_string() || !storyGoalTypeMap.contains(table["type"].as_string()->get())) {
            Log::Warning("Story goal {} has an invalid type. Available: Databank, Player, World", id);
            return;
        }
        factory.setTargetType(storyGoalTypeMap.at(table["type"].as_string()->get()));
    }

    if (const auto result = factory.registerStoryGoal(); result == nullptr)
        Log::Warning("Story goal {} failed to register for an unknown reason", id);
    else
        storyGoals.insert(std::make_pair(id, result));
}

void StoryGoalParser::ParseStoryGoals() {
    for (const auto&[mod, path, toml] : FileTraversal::storyGoalTables) {
        if (!toml["story_goal"].is_array()) {
            Log::Warning("Malformed story goal file {}", path);
            return;
        }

        for (const auto& goal : *toml["story_goal"].as_array()) {
            if (!goal.is_table()) {
                Log::Warning("Malformed story goal file {}", path);
                return;
            }
            parseFile(path, *goal.as_table());
        }
    }
}
