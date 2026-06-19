//
// Created by Limo on 19/06/2026.
//

#pragma once

#include <string>
#include <variant>

#include "SDK/UWEStoryGoals_classes.hpp"

enum StoryGoalRuleType {
    And,
    Or,
    Negate,
    Count,
    Required
};

struct StoryGoalRule {
    std::string goalId = "";

    virtual ~StoryGoalRule() = default;
    virtual StoryGoalRuleType type() const = 0;
};

struct StoryGoalRuleAnd : StoryGoalRule {
    std::vector<StoryGoalRule> rules;

    StoryGoalRuleType type() const override { return And; }
};

struct StoryGoalRuleOr : StoryGoalRule {
    std::vector<StoryGoalRule> rules;

    StoryGoalRuleType type() const override { return Or; }
};

struct StoryGoalRuleNegate : StoryGoalRule {
    std::unique_ptr<StoryGoalRule> rule;

    StoryGoalRuleType type() const override { return Negate; }
};

struct StoryGoalRuleCount : StoryGoalRule {
    std::vector<StoryGoalRule> rules;
    int count;

    StoryGoalRuleType type() const override { return Count; }
};

struct StoryGoalRequiredRule : StoryGoalRule {
    SDK::UUWEStoryGoal* goal;

    StoryGoalRuleType type() const override { return Required; }
};

class StoryGoalFactory {
    template <typename T, typename = std::enable_if_t<std::is_base_of_v<SDK::UUWEStoryGoalRule, T>>>
    static T* registerBase(std::string baseId, std::string id, int* incrementor, std::optional<SDK::UObject*> outer);

    static void addToArray(SDK::TArray<SDK::UUWEStoryGoalRule *> array, const std::vector<StoryGoalRule> &rules, int *incrementor, std::optional<SDK::UObject*> outer);

    static SDK::UUWEStoryGoalRule* registerUnknown(const StoryGoalRule& rule, int* incrementor, std::optional<SDK::UObject*> outer);
    static SDK::UUWEStoryGoalRule* registerAnd(const StoryGoalRuleAnd *rule, int *incrementor, std::optional<SDK::UObject*> outer);
    static SDK::UUWEStoryGoalRule* registerOr(const StoryGoalRuleOr* rule, int* incrementor, std::optional<SDK::UObject*> outer);
    static SDK::UUWEStoryGoalRule* registerNegate(const StoryGoalRuleNegate* rule, int* incrementor, std::optional<SDK::UObject*> outer);
    static SDK::UUWEStoryGoalRule* registerCount(const StoryGoalRuleCount* rule, int* incrementor, std::optional<SDK::UObject*> outer);
    static SDK::UUWEStoryGoalRule* registerRequired(const StoryGoalRequiredRule* rule, int *incrementor, std::optional<SDK::UObject*> outer);

public:
    static SDK::UUWEStoryGoalRule* registerStoryGoalRule(const StoryGoalRule& rule);
};
