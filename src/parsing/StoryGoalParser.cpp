//
// Created by Limo on 19/06/2026.
//

#include "StoryGoalParser.hpp"

#include "FileTraversal.hpp"
#include "registering/StoryGoalRuleFactory.hpp"
#include "util/Log.hpp"

std::map<std::string, UUWEStoryGoal*> StoryGoalParser::storyGoals{};

const std::map<std::string, EStoryGoalTargetType> storyGoalTypeMap = {
    { "Databank", EStoryGoalTargetType::Databank },
    { "Player", EStoryGoalTargetType::Player },
    { "World", EStoryGoalTargetType::World }
};

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
