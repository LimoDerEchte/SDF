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
    std::optional<SDK::TSoftObjectPtr<SDK::UTexture2D>> icon;

public:
    explicit BioModFactory(std::string bioModId, bool isModify);

    void setName(const std::string& bioModName);
    void setDescription(const std::string& bioModDescription);
    void setType(SDK::EUWEBioAbilityType bioModType);

    bool setIcon(SDK::UTexture2D *bioModIcon);
    void setIcon(const SDK::TSoftObjectPtr<SDK::UTexture2D> &bioModIcon);

    [[nodiscard]] SDK::UUWEBioAbilityData* registerBioMod();
};
