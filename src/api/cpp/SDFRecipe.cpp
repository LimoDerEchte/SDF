//
// Created by Limo on 28/06/2026.
//

#include "SDFRecipe.hpp"

SDFRecipe_Impl::SDFRecipe_Impl(const std::string &id, const bool modifyMode) : factory(id, modifyMode) { }

void SDFRecipe_Impl::setName(const std::string &name) {
    factory.setName(name);
}

void SDFRecipe_Impl::setDescription(const std::string &description) {
    factory.setDescription(description);
}

void SDFRecipe_Impl::setCategory(const std::string &category) {
    factory.setCategory(category);
}

void SDFRecipe_Impl::setCategory(RC::Unreal::UObject *category) {
    factory.setCategory(reinterpret_cast<SDK::UUWECraftingRecipeCategory*>(category));
}

void SDFRecipe_Impl::setIcon(RC::Unreal::UObject *icon) {
    factory.setIcon(reinterpret_cast<SDK::UTexture2D*>(icon));
}

void SDFRecipe_Impl::setIcon(const RC::Unreal::TSoftObjectPtr<> &icon) {
    factory.setIcon(reinterpret_cast<const SDK::TSoftObjectPtr<SDK::UTexture2D>&>(icon));
}

void SDFRecipe_Impl::addIngredient(const std::string &ingredient, const int32_t amount) {
    factory.addIngredient(ingredient, amount);
}

void SDFRecipe_Impl::addIngredient(RC::Unreal::UObject *ingredient, const int32_t amount) {
    factory.addIngredient(reinterpret_cast<SDK::UUWEItemType*>(ingredient), amount);
}

void SDFRecipe_Impl::addOutput(const std::string &output, const int32_t amount) {
    factory.addOutput(output, amount);
}

void SDFRecipe_Impl::addOutput(RC::Unreal::UObject *output, const int32_t amount) {
    factory.addOutput(reinterpret_cast<SDK::UUWEItemType*>(output), amount);
}

void SDFRecipe_Impl::addItemPickupUnlockingRequirement(const std::string &setId, const std::string &item) {
    factory.addUnlockingRequirementPickup(setId, item);
}

void SDFRecipe_Impl::addItemPickupUnlockingRequirement(const std::string &setId, RC::Unreal::UObject *item) {
    factory.addUnlockingRequirementPickup(setId, reinterpret_cast<SDK::UUWEItemType*>(item));
}

void SDFRecipe_Impl::addScanDataUnlockingRequirement(const std::string &setId, const std::string &scanData) {
    factory.addUnlockingRequirementScanData(setId, scanData);
}

void SDFRecipe_Impl::addScanDataUnlockingRequirement(const std::string &setId, RC::Unreal::UObject *scanData) {
    factory.addUnlockingRequirementScanData(setId, reinterpret_cast<SDK::UUWEScanData*>(scanData));
}

void SDFRecipe_Impl::removeUnlockingRequirements() {
    factory.modifyRemoveRequirements();
}

void SDFRecipe_Impl::setCraftingTime(const float time) {
    factory.setCraftingTime(time);
}

void SDFRecipe_Impl::setAvailableInLifepod(const bool available) {
    factory.setAvailableInLifepod(available);
}

void SDFRecipe_Impl::setOrderingIndex(const int orderingIndex) {
    factory.setOrderingIndex(orderingIndex);
}

RC::Unreal::UObject *SDFRecipe_Impl::createOrModifyRecipe() {
    return reinterpret_cast<RC::Unreal::UObject*>(factory.registerRecipe());
}
