//
// Created by Limo on 13/06/2026.
//

#include "DatabankEntryFactory.hpp"

#include "UKismetSystemLibrary.hpp"d

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

bool DatabankEntryFactory::setIcon(UTexture2D *newIcon) {
    if (newIcon == nullptr)
        return false;
    iconModified = true;
    *reinterpret_cast<Unreal::TSoftObjectPtr<>*>(&icon) = Unreal::UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(reinterpret_cast<Unreal::UObject*>(newIcon));
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
        entry->EntryImage = icon;
    if (!modifyMode || title.has_value())
        *reinterpret_cast<Unreal::FText*>(&entry->EntryTitle) = Unreal::FText(UtfN::StringToWString(title.value_or("Empty")).c_str());
    if (!modifyMode || text.has_value())
        *reinterpret_cast<Unreal::FText*>(&entry->EntryText) = Unreal::FText(UtfN::StringToWString(text.value_or("Empty")).c_str());

    if (categoriesModified) {
        const auto categoryList = reinterpret_cast<Unreal::TArray<Unreal::FText>*>(&entry->Categories);
        if (categoryList->Num() > 0)
            categoryList->Empty();

        categoryList->ResizeTo(static_cast<int32_t>(categories.size()));
        for (const auto &category : categories)
            categoryList->Add(Unreal::FText(UtfN::StringToWString(category).c_str()));
    }

    if (unlockConditionModified)
        entry->UnlockingRequirements = unlockCondition;
    if (hideConditionModified)
        entry->HideOnStoryGoal = hideCondition;

    RegistryHelper::AddToRegistry(entry, "UWEDatabankEntry");
    registeredDatabankEntries.push_back(entry);

    Log::Verbose("Databank entry {}: {}", modifyMode ? "modified" : "registered", id);
    SDF_Impl::TriggerCreateAsset(SDF::Category, id, reinterpret_cast<Unreal::UObject*>(entry));
    return entry;
}
