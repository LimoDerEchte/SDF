//
// Created by Limo on 19/06/2026.
//

#include "StoryGoalRuleFactory.hpp"

#include "util/Finders.hpp"
#include "util/RegistryHelper.hpp"
#include "Containers/Array.hpp"
#include "util/Log.hpp"

std::vector<UUWEStoryGoal*> StoryGoalFactory::registeredStoryGoals{};

template<typename T, typename>
T *StoryGoalRuleFactory::registerBase(const std::string baseId, std::string id, int *incrementor, const std::optional<UObject*> outer) {
    const auto base = Finders::searchFirstOf<T>(baseId);
    if (base == nullptr)
        return nullptr;

    auto name = std::format("DA_{}", id);
    if (outer.has_value())
        name = std::format("{}_SDF_{}", baseId, (*incrementor)++);

    return RegistryHelper::StaticConstructTemplate(base, name, outer);
}

void StoryGoalRuleFactory::addToArray(TArray<UUWEStoryGoalRule*>* array, const std::vector<std::unique_ptr<StoryGoalRule>> &rules, int *incrementor, const std::optional<UObject*> outer) {
    if (array->Num() > 0)
        array->Empty();

    array->ResizeTo(static_cast<int32_t>(rules.size()));
    for (auto &ruleEntry : rules)
        array->Add(registerUnknown(ruleEntry, incrementor, outer));
}

UUWEStoryGoalRule *StoryGoalRuleFactory::registerUnknown(const std::unique_ptr<StoryGoalRule> &rule, int *incrementor, const std::optional<UObject*> outer) {
    if (rule == nullptr)
        return nullptr;
    switch (rule->type()) {
        case And:
            return registerAnd(reinterpret_cast<const StoryGoalRuleAnd*>(rule.get()), incrementor, outer);
        case Or:
            return registerOr(reinterpret_cast<const StoryGoalRuleOr*>(rule.get()), incrementor, outer);
        case Negate:
            return registerNegate(reinterpret_cast<const StoryGoalRuleNegate*>(rule.get()), incrementor, outer);
        case Count:
            return registerCount(reinterpret_cast<const StoryGoalRuleCount*>(rule.get()), incrementor, outer);
        case Required:
            return registerRequired(reinterpret_cast<const StoryGoalRequiredRule*>(rule.get()), incrementor, outer);
    }
    return nullptr;
}

UUWEStoryGoalRule *StoryGoalRuleFactory::registerAnd(const StoryGoalRuleAnd *rule, int *incrementor, const std::optional<UObject*> outer) {
    const auto entry = registerBase<UUWEStoryGoalRuleAnd>("UWEStoryGoalRuleAnd", rule->goalId, incrementor, outer);
    addToArray(entry->GetRules(), rule->rules, incrementor, outer.value_or(entry));
    return entry;
}

UUWEStoryGoalRule *StoryGoalRuleFactory::registerOr(const StoryGoalRuleOr *rule, int *incrementor, const std::optional<UObject*> outer) {
    const auto entry = registerBase<UUWEStoryGoalRuleOr>("UWEStoryGoalRuleOr", rule->goalId, incrementor, outer);
    addToArray(entry->GetRules(), rule->rules, incrementor, outer.value_or(entry));
    return entry;
}

UUWEStoryGoalRule *StoryGoalRuleFactory::registerNegate(const StoryGoalRuleNegate *rule, int *incrementor, const std::optional<UObject*> outer) {
    const auto entry = registerBase<UUWEStoryGoalRuleNegate>("UWEStoryGoalRuleNegate", rule->goalId, incrementor, outer);
    entry->SetRuleToNegate(registerUnknown(rule->rule, incrementor, outer.value_or(entry)));
    return entry;
}

UUWEStoryGoalRule *StoryGoalRuleFactory::registerCount(const StoryGoalRuleCount *rule, int *incrementor, const std::optional<UObject*> outer) {
    const auto entry = registerBase<UUWEStoryGoalRuleCount>("UWEStoryGoalRuleCount", rule->goalId, incrementor, outer);
    addToArray(entry->GetRules(), rule->rules, incrementor, outer.value_or(entry));
    entry->SetMinimumCount(rule->count);
    return entry;
}

UUWEStoryGoalRule *StoryGoalRuleFactory::registerRequired(const StoryGoalRequiredRule *rule, int *incrementor, const std::optional<UObject*> outer) {
    const auto entry = registerBase<UUWERequiredStoryGoalRule>("UWERequiredStoryGoalRule", rule->goalId, incrementor, outer);
    entry->SetRequiredStoryGoalRef(rule->goal);
    return entry;
}

UUWEStoryGoalRule *StoryGoalRuleFactory::registerStoryGoalRule(const std::unique_ptr<StoryGoalRule> &rule) {
    int incrementor = 0;
    return registerUnknown(rule, &incrementor, std::nullopt);
}

StoryGoalFactory::StoryGoalFactory(const std::string &id)
    : id(id) {
}

void StoryGoalFactory::setTargetType(const EStoryGoalTargetType type) {
    this->type = type;
}

UUWEStoryGoal *StoryGoalFactory::registerStoryGoal() {
    const auto base = Finders::searchFirstOf<UUWEStoryGoal>("UWEStoryGoal");
    if (base == nullptr)
        return nullptr;

    const auto entry = RegistryHelper::StaticConstructTemplate(base, std::format("DA_StoryGoal_{}", id));
    if (entry == nullptr)
        return nullptr;

    entry->StoryGoalType = type;

    RegistryHelper::AddToRegistry(entry, "UWEStoryGoal");
    registeredStoryGoals.push_back(entry);

    Log::Verbose("Story goal registered: {}", id);
    return entry;
}
