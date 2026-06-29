//
// Created by Limo on 28/06/2026.
//

#include "SDFCategory.hpp"

SDFCategory_Impl::SDFCategory_Impl(const std::string &id, const bool modifyMode) : factory(id, modifyMode) { }

void SDFCategory_Impl::setName(const std::string &name) {
    factory.setName(name);
}

void SDFCategory_Impl::setDescription(const std::string &description) {
    factory.setDescription(description);
}

void SDFCategory_Impl::setCrafterType(uint8_t type) {
    factory.setCrafterType(static_cast<SDK::ECrafterType>(type));
}

void SDFCategory_Impl::setParent(const std::string &category) {
    factory.setParent(category);
}

void SDFCategory_Impl::setParent(RC::Unreal::UObject *category) {
    factory.setParent(reinterpret_cast<SDK::UUWECraftingRecipeCategory*>(category));
}

void SDFCategory_Impl::setIcon(RC::Unreal::UObject *icon) {
    factory.setIcon(reinterpret_cast<SDK::UTexture2D*>(icon));
}

void SDFCategory_Impl::setIcon(const RC::Unreal::TSoftObjectPtr<> &icon) {
    factory.setIcon(reinterpret_cast<const SDK::TSoftObjectPtr<SDK::UTexture2D>&>(icon));
}

void SDFCategory_Impl::setOrderingIndex(const int orderingIndex) {
    factory.setOrderingIndex(orderingIndex);
}

RC::Unreal::UObject *SDFCategory_Impl::createOrModifyCategory() {
    return reinterpret_cast<RC::Unreal::UObject*>(factory.registerCategory());
}
