//
// Created by limo on 26.06.26.
//

#include "BioModFactory.hpp"

#include "util/Finders.hpp"
#include "util/Log.hpp"
#include "util/RegistryHelper.hpp"

using namespace SDK;

std::vector<UUWEBioAbilityData*> BioModFactory::registeredBioMods;

BioModFactory::BioModFactory(std::string bioModId, const bool isModify)
    : modifyMode(isModify), id(std::move(bioModId)) {
}

void BioModFactory::setName(const std::string &name) {
    this->name = name;
}

void BioModFactory::setDescription(const std::string &description) {
    this->description = description;
}

void BioModFactory::setType(EUWEBioAbilityType type) {
    this->type = type;
}

UUWEBioAbilityData* BioModFactory::registerBioMod() {
    const auto base = Finders::searchFirstOf<UUWEBioAbilityData>("UWEBioAbilityData");
    if (base == nullptr)
        return nullptr;

    const auto bioMod = modifyMode ? Finders::searchBioAbilityData(id) : RegistryHelper::StaticConstructTemplate(base, "DA_" + id + "_BioAbilityData");
    if (bioMod == nullptr)
        return nullptr;

    if (!modifyMode || !name->empty())
        bioMod->Name_0 = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(name.value_or("Invalid")).c_str());
    if (!modifyMode || !description->empty())
        bioMod->Description = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(description.value_or("Invalid")).c_str());

    RegistryHelper::AddToRegistry(bioMod, "UWEBioAbilityData");
    registeredBioMods.push_back(bioMod);

    Log::Verbose("Recipe {}: {}", modifyMode ? "modified" : "registered", id);
    return bioMod;
}
