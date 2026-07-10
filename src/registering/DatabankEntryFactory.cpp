//
// Created by Limo on 13/06/2026.
//

#include "DatabankEntryFactory.hpp"

#include "UKismetSystemLibrary.hpp"

#include "util/RegistryHelper.hpp"
#include "util/Finders.hpp"
#include "util/Log.hpp"

#include "Containers/Array.hpp"
#include "api/cpp/SDF.hpp"

using namespace RC;

std::vector<UUWEDatabankEntry*> DatabankEntryFactory::registeredDatabankEntries;

DatabankEntryFactory::DatabankEntryFactory(const std::string &id, const bool modifyMode)
    : id(id), icon(), modifyMode(modifyMode) {
}

void DatabankEntryFactory::setTitle(const std::string &newTitle) {
    title = newTitle;
}

void DatabankEntryFactory::setText(const std::string &newText) {
    text = newText;
}

void DatabankEntryFactory::addCategory(const std::string &category) {
    categoriesModified = true;
    categories.push_back(category);
}

bool DatabankEntryFactory::setIcon(const UTexture2D *newIcon) {
    if (newIcon == nullptr)
        return false;
    iconModified = true;
    icon = newIcon;
    return true;
}

void DatabankEntryFactory::setIcon(const TSoftObjectPtr<UTexture2D> &newIcon) {
    iconModified = true;
    icon = newIcon;
}

bool DatabankEntryFactory::setUnlockCondition(const std::string &ruleId) {
    return setUnlockCondition(Finders::searchStoryGoalRule(ruleId));
}

bool DatabankEntryFactory::setUnlockCondition(UUWEStoryGoalRule *goalRule) {
    if (goalRule == nullptr)
        return false;
    unlockCondition = goalRule;
    unlockConditionModified = true;
    return true;
}

bool DatabankEntryFactory::setHideCondition(const std::string &goalId) {
    return setHideCondition(Finders::searchStoryGoal(goalId));
}

bool DatabankEntryFactory::setHideCondition(UUWEStoryGoal *goal) {
    if (goal == nullptr)
        return false;
    hideCondition = goal;
    hideConditionModified = true;
    return true;
}

UUWEDatabankEntry *DatabankEntryFactory::registerDatabankEntry() {
    const auto base = Finders::searchFirstOf<UUWEDatabankEntry>("UWEDatabankEntry");
    if (base == nullptr)
        return nullptr;

    const auto entry = modifyMode ? Finders::searchDatabankEntry(id) : RegistryHelper::StaticConstructTemplate(base, std::format("DA_{}_DatabankEntry", id));
    if (entry == nullptr)
        return nullptr;

    if (iconModified)
        entry->SetEntryImage(icon);
    if (!modifyMode || title.has_value())
        entry->SetEntryTitle(FText(UtfN::StringToWString(title.value_or("Empty")).c_str()));
    if (!modifyMode || text.has_value())
        entry->SetEntryText(FText(UtfN::StringToWString(text.value_or("Empty")).c_str()));

    if (categoriesModified) {
        const auto categoryList = entry->GetCategories();
        if (categoryList->Num() > 0)
            categoryList->Empty();

        categoryList->ResizeTo(static_cast<int32_t>(categories.size()));
        for (const auto &category : categories)
            categoryList->Add(Unreal::FText(UtfN::StringToWString(category).c_str()));
    }

    if (unlockConditionModified)
        entry->SetUnlockingRequirements(unlockCondition);
    if (hideConditionModified)
        entry->SetHideOnStoryGoal(hideCondition);

    RegistryHelper::AddToRegistry(entry, "UWEDatabankEntry");
    registeredDatabankEntries.push_back(entry);

    Log::Verbose("Databank entry {}: {}", modifyMode ? "modified" : "registered", id);
    SDF_Impl::TriggerCreateAsset(SDF::Category, id, entry);
    return entry;
}
