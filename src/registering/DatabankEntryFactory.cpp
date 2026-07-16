//
// Created by Limo on 13/06/2026.
//

#include "DatabankEntryFactory.hpp"

#include <utility>

#include "UKismetSystemLibrary.hpp"

#include "util/RegistryHelper.hpp"
#include "util/Finders.hpp"
#include "util/Log.hpp"

#include "Containers/Array.hpp"
#include "api/cpp/SDF.hpp"

using namespace RC;

std::vector<UUWEDatabankEntry*> DatabankEntryFactory::registeredDatabankEntries;

DatabankEntryFactory::DatabankEntryFactory(std::string id, const bool modifyMode)
    : id(std::move(id)), modifyMode(modifyMode) {
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

bool DatabankEntryFactory::setIcon(UTexture2D *newIcon) {
    if (newIcon == nullptr)
        return false;
    icon = newIcon;
    return true;
}

bool DatabankEntryFactory::setUnlockCondition(const std::string &ruleId) {
    return setUnlockCondition(Finders::searchStoryGoalRule(ruleId));
}

bool DatabankEntryFactory::setUnlockCondition(UUWEStoryGoalRule *goalRule) {
    if (goalRule == nullptr)
        return false;
    unlockCondition = goalRule;
    return true;
}

bool DatabankEntryFactory::setHideCondition(const std::string &goalId) {
    return setHideCondition(Finders::searchStoryGoal(goalId));
}

bool DatabankEntryFactory::setHideCondition(UUWEStoryGoal *goal) {
    if (goal == nullptr)
        return false;
    hideCondition = goal;
    return true;
}

UUWEDatabankEntry *DatabankEntryFactory::registerDatabankEntry() {
    const auto base = Finders::searchFirstOf<UUWEDatabankEntry>("UWEDatabankEntry");
    if (base == nullptr)
        return nullptr;

    const auto entry = modifyMode ? Finders::searchDatabankEntry(id) : RegistryHelper::StaticConstructTemplate(base, std::format("DA_{}_DatabankEntry", id));
    if (entry == nullptr)
        return nullptr;

    Log::Warning("Test: {}", UtfN::WStringToString(base->GetFullName()));
    Log::Warning("Test 0 {:p}", (void*)base);
    const auto sptr = base->GetEntryImage();
    Log::Warning("Test 1 {:p}", (void*)sptr);
    const auto icon = sptr->IsValid();
    Log::Warning("Test 2 {}", icon);
    //Log::Warning("Test 2 {:p}", (void*)icon);
    //entry->SetEntryImage(icon);

    //if (icon.has_value())
    //    entry->SetEntryImage();
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
            categoryList->Add(FText(UtfN::StringToWString(category).c_str()));
    }

    if (unlockCondition.has_value())
        entry->SetUnlockingRequirements(unlockCondition.value());
    if (hideCondition.has_value())
        entry->SetHideOnStoryGoal(hideCondition.value());

    RegistryHelper::AddToRegistry(entry, "UWEDatabankEntry");
    registeredDatabankEntries.push_back(entry);

    Log::Verbose("Databank entry {}: {}", modifyMode ? "modified" : "registered", id);
    SDF_Impl::TriggerCreateAsset(SDF::Category, id, entry);
    return entry;
}
