//
// Created by Limo on 19/06/2026.
//

#pragma once

#include "sdk/Types.hpp"

enum StoryGoalRuleType {
    And,
    Or,
    Negate,
    Count,
    Required
};

struct StoryGoalRule {
    std::string goalId;

    virtual ~StoryGoalRule() = default;
    [[nodiscard]] virtual StoryGoalRuleType type() const = 0;
};

struct StoryGoalRuleListType : StoryGoalRule {
    std::vector<std::unique_ptr<StoryGoalRule>> rules;
};

struct StoryGoalRuleAnd : StoryGoalRuleListType {
    [[nodiscard]] StoryGoalRuleType type() const override { return And; }
};

struct StoryGoalRuleOr : StoryGoalRuleListType {
    [[nodiscard]] StoryGoalRuleType type() const override { return Or; }
};

struct StoryGoalRuleNegate : StoryGoalRule {
    std::unique_ptr<StoryGoalRule> rule;

    [[nodiscard]] StoryGoalRuleType type() const override { return Negate; }
};

struct StoryGoalRuleCount : StoryGoalRuleListType {
    int count = 0;

    [[nodiscard]] StoryGoalRuleType type() const override { return Count; }
};

struct StoryGoalRequiredRule : StoryGoalRule {
    UUWEStoryGoal* goal;

    [[nodiscard]] StoryGoalRuleType type() const override { return Required; }
};

class StoryGoalRuleFactory {
    template <typename T, typename = std::enable_if_t<std::is_base_of_v<UUWEStoryGoalRule, T>>>
    static T* registerBase(std::string baseId, std::string id, int* incrementor, std::optional<UObject*> outer);

    static void addToArray(TArray<UUWEStoryGoalRule *> *array, const std::vector<std::unique_ptr<StoryGoalRule>> &rules, int *incrementor, std::optional<UObject*> outer);

    static UUWEStoryGoalRule* registerUnknown(const std::unique_ptr<StoryGoalRule> &rule, int* incrementor, std::optional<UObject*> outer);
    static UUWEStoryGoalRule* registerAnd(const StoryGoalRuleAnd *rule, int *incrementor, std::optional<UObject *> outer);
    static UUWEStoryGoalRule* registerOr(const StoryGoalRuleOr *rule, int *incrementor, std::optional<UObject *> outer);
    static UUWEStoryGoalRule* registerNegate(const StoryGoalRuleNegate *rule, int *incrementor, std::optional<UObject *> outer);
    static UUWEStoryGoalRule* registerCount(const StoryGoalRuleCount *rule, int *incrementor, std::optional<UObject *> outer);
    static UUWEStoryGoalRule* registerRequired(const StoryGoalRequiredRule *rule, int *incrementor, std::optional<UObject *> outer);

public:
    static UUWEStoryGoalRule* registerStoryGoalRule(const std::unique_ptr<StoryGoalRule> &rule);
};

class StoryGoalFactory {
    static std::vector<UUWEStoryGoal*> registeredStoryGoals;

    std::string id;
    EStoryGoalTargetType type = EStoryGoalTargetType::NotSet;

public:
    explicit StoryGoalFactory(const std::string& id);

    void setTargetType(EStoryGoalTargetType type);

    UUWEStoryGoal* registerStoryGoal();
};
