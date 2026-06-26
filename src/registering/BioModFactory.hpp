//
// Created by limo on 26.06.26.
//

#pragma once

#include <vector>

#include "SDK/UWEBiomods_classes.hpp"

class BioModFactory {
    static std::vector<SDK::UUWEBioAbilityData*> registeredBioMods;

    bool modifyMode = false;

    std::string id;
    std::optional<std::string> name, description;

    std::optional<SDK::EUWEBioAbilityType> type;

public:
    explicit BioModFactory(std::string bioModId, bool isModify);

    void setName(const std::string& name);
    void setDescription(const std::string& description);
    void setType(SDK::EUWEBioAbilityType type);

    [[nodiscard]] SDK::UUWEBioAbilityData* registerBioMod();
};
