//
// Created by Limo on 19/06/2026.
//

#include "StoryGoalFactory.hpp"

#include "util/Finders.hpp"
#include "util/RegistryHelper.hpp"

using namespace SDK;

template<typename T, typename>
T *StoryGoalFactory::registerBase(const std::string baseId, std::string id, int *incrementor, const std::optional<UObject*> outer) {
    const auto base = Finders::searchFirstOf<T>(baseId);
    if (base == nullptr)
        return nullptr;

    auto name = std::format("DA_{}", id);
    if (outer.has_value())
        name = std::format("{}_SDF_{}", baseId, (*incrementor)++);

    return RegistryHelper::StaticConstructTemplate(base, name, outer);
}

void StoryGoalFactory::addToArray(TArray<UUWEStoryGoalRule*> array, const std::vector<StoryGoalRule> &rules, int *incrementor, const std::optional<UObject*> outer) {
    const auto ruleList = reinterpret_cast<RC::Unreal::TArray<UUWEStoryGoalRule*>*>(&array);
    if (ruleList->Num() > 0)
        ruleList->Empty();

    ruleList->ResizeTo(static_cast<int32_t>(rules.size()));
    for (const auto &ruleEntry : rules)
        ruleList->Add(registerUnknown(ruleEntry, incrementor, outer));
}

UUWEStoryGoalRule *StoryGoalFactory::registerUnknown(const StoryGoalRule &rule, int *incrementor, const std::optional<UObject*> outer) {
    switch (rule.type()) {
        case And:
            return registerAnd(reinterpret_cast<const StoryGoalRuleAnd*>(&rule), incrementor, outer);
        case Or:
            return registerOr(reinterpret_cast<const StoryGoalRuleOr*>(&rule), incrementor, outer);
        case Negate:
            return registerNegate(reinterpret_cast<const StoryGoalRuleNegate*>(&rule), incrementor, outer);
        case Count:
            return registerCount(reinterpret_cast<const StoryGoalRuleCount*>(&rule), incrementor, outer);
        case Required:
            return registerRequired(reinterpret_cast<const StoryGoalRequiredRule*>(&rule), incrementor, outer);
    }
    return nullptr;
}

UUWEStoryGoalRule *StoryGoalFactory::registerAnd(const StoryGoalRuleAnd *rule, int *incrementor, const std::optional<UObject*> outer) {
    const auto entry = registerBase<UUWEStoryGoalRuleAnd>("UWEStoryGoalRuleAnd", rule->goalId, incrementor, outer);
    addToArray(entry->Rules, rule->rules, incrementor, outer);
    return entry;
}

UUWEStoryGoalRule *StoryGoalFactory::registerOr(const StoryGoalRuleOr *rule, int *incrementor, const std::optional<UObject*> outer) {
    const auto entry = registerBase<UUWEStoryGoalRuleOr>("UWEStoryGoalRuleOr", rule->goalId, incrementor, outer);
    addToArray(entry->Rules, rule->rules, incrementor, outer);
    return entry;
}

UUWEStoryGoalRule *StoryGoalFactory::registerNegate(const StoryGoalRuleNegate *rule, int *incrementor, const std::optional<UObject*> outer) {
    const auto entry = registerBase<UUWEStoryGoalRuleNegate>("UWEStoryGoalRuleNegate", rule->goalId, incrementor, outer);
    entry->RuleToNegate = registerUnknown(*rule->rule, incrementor, outer);
    return entry;
}

UUWEStoryGoalRule *StoryGoalFactory::registerCount(const StoryGoalRuleCount *rule, int *incrementor, const std::optional<UObject*> outer) {
    const auto entry = registerBase<UUWEStoryGoalRuleCount>("UWEStoryGoalRuleCount", rule->goalId, incrementor, outer);
    addToArray(entry->Rules, rule->rules, incrementor, outer);
    entry->MinimumCount = rule->count;
    return entry;
}

UUWEStoryGoalRule *StoryGoalFactory::registerRequired(const StoryGoalRequiredRule *rule, int *incrementor, const std::optional<UObject*> outer) {
    const auto entry = registerBase<UUWERequiredStoryGoalRule>("UWERequiredStoryGoalRule", rule->goalId, incrementor, outer);
    entry->RequiredStoryGoalRef = rule->goal;
    return entry;
}

UUWEStoryGoalRule *StoryGoalFactory::registerStoryGoalRule(const StoryGoalRule &rule) {
    int incrementor = 0;
    return registerUnknown(rule, &incrementor, std::nullopt);
}
