//
// Created by limo on 26.06.26.
//

#include "BioModFactory.hpp"

#include "FText.hpp"
#include "UKismetSystemLibrary.hpp"
#include "sdk/TempFinders.hpp"
#include "util/Finders.hpp"
#include "util/Log.hpp"
#include "util/RegistryHelper.hpp"

using namespace SDK;
using namespace RC;
using namespace Unreal;

std::vector<UUWEBioAbilityData*> BioModFactory::registeredBioMods;

BioModFactory::BioModFactory(std::string bioModId, const bool isModify)
    : modifyMode(isModify), id(std::move(bioModId)) {
    if (!isModify)
        setIcon(Finders::searchTexture("T_DefaultImage"));
}

void BioModFactory::setName(const std::string &bioModName) {
    name = bioModName;
}

void BioModFactory::setDescription(const std::string &bioModDescription) {
    description = bioModDescription;
}

void BioModFactory::setType(EUWEBioAbilityType bioModType) {
    type = bioModType;
}

bool BioModFactory::setIcon(UTexture2D *bioModIcon) {
    if (bioModIcon == nullptr)
        return false;
    *reinterpret_cast<Unreal::TSoftObjectPtr<>*>(&icon) = Unreal::UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(reinterpret_cast<Unreal::UObject*>(bioModIcon));
    return true;
}

void BioModFactory::setIcon(const SDK::TSoftObjectPtr<UTexture2D> &bioModIcon) {
    icon = bioModIcon;
}

UUWEBioAbilityData* BioModFactory::registerBioMod() {
    const auto base = Finders::searchFirstOf<UUWEBioAbilityData>("UWEBioAbilityData");
    if (base == nullptr)
        return nullptr;

    const auto bioMod = modifyMode ? Finders::searchBioAbilityData(id) : RegistryHelper::StaticConstructTemplate(base, "DA_" + id + "_BioAbilityData");
    if (bioMod == nullptr)
        return nullptr;

    if (!modifyMode) {
        bioMod->PublishedStatus = EUWEPublishedStatus::Published;
        bioMod->AbilityTag = TempFinders::CreateTag(std::format("Biomods.BioAbility.{}", id));
    }

    if (!modifyMode || name.has_value())
        *reinterpret_cast<Unreal::FText*>(&bioMod->Name_0) = Unreal::FText(UtfN::StringToWString(name.value_or("Invalid")).c_str());
    if (!modifyMode || description.has_value())
        *reinterpret_cast<Unreal::FText*>(&bioMod->Description) = Unreal::FText(UtfN::StringToWString(description.value_or("Invalid")).c_str());

    if (!modifyMode || type.has_value())
        bioMod->BioAbilityType = type.value_or(EUWEBioAbilityType::Passive);
    if (icon.has_value())
        bioMod->Icon = icon.value();

    RegistryHelper::AddToRegistry(bioMod, "UWEBioAbilityData");
    registeredBioMods.push_back(bioMod);

    Log::Verbose("Recipe {}: {}", modifyMode ? "modified" : "registered", id);
    return bioMod;
}
