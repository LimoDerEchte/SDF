//
// Created by Limo on 28/06/2026.
//

#pragma once

#include "registering/RecipeFactory.hpp"
#include "SDF/SDFRecipe.hpp"

class SDFRecipe_Impl final : public SDFRecipe {
    RecipeFactory factory;

public:
    explicit SDFRecipe_Impl(const std::string &id, bool modifyMode);

    void setName(const std::string &name) override;
    void setDescription(const std::string &description) override;

    void setCategory(const std::string &category) override;
    void setCategory(RC::Unreal::UObject *category) override;

    void setIcon(const std::string &icon) override;
    void setIcon(RC::Unreal::UObject *icon) override;
    void setIcon(const RC::Unreal::TSoftObjectPtr<> &icon) override;

    void addIngredient(const std::string &ingredient, int32_t amount) override;
    void addIngredient(RC::Unreal::UObject *ingredient, int32_t amount) override;

    void addOutput(const std::string &output, int32_t amount) override;
    void addOutput(RC::Unreal::UObject *output, int32_t amount) override;

    void addItemPickupUnlockingRequirement(const std::string &setId, const std::string &item) override;
    void addItemPickupUnlockingRequirement(const std::string &setId, RC::Unreal::UObject *item) override;

    void addScanDataUnlockingRequirement(const std::string &setId, const std::string &scanData) override;
    void addScanDataUnlockingRequirement(const std::string &setId, RC::Unreal::UObject *scanData) override;

    void removeUnlockingRequirements() override;

    void setCraftingTime(float time) override;
    void setAvailableInLifepod(bool available) override;
    void setOrderingIndex(int orderingIndex) override;

    [[nodiscard]] RC::Unreal::UObject *createOrModifyRecipe() override;
};
