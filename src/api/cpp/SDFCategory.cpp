//
// Created by Limo on 28/06/2026.
//

#include "SDFCategory.hpp"

#include "parsing/generic/IconParser.hpp"

SDFCategory_Impl::SDFCategory_Impl(const std::string &id, const bool modifyMode) : factory(id, modifyMode) { }

void SDFCategory_Impl::setName(const std::string &name) {
    factory.setName(name);
}

void SDFCategory_Impl::setDescription(const std::string &description) {
    factory.setDescription(description);
}

void SDFCategory_Impl::setCrafterType(uint8_t type) {
    factory.setCrafterType(static_cast<ECrafterType>(type));
}

void SDFCategory_Impl::setParent(const std::string &category) {
    factory.setParent(category);
}

void SDFCategory_Impl::setParent(UObject *category) {
    factory.setParent(reinterpret_cast<UUWECraftingRecipeCategory*>(category));
}

void SDFCategory_Impl::setIcon(const std::string &icon) {
    switch (const IconParser parser(icon, ""); parser.getResult()) {
        case Success:
            factory.setIcon(parser.getTexture());
            break;
        default:
    }
}

void SDFCategory_Impl::setIcon(UObject *icon) {
    factory.setIcon(reinterpret_cast<UTexture2D*>(icon));
}

void SDFCategory_Impl::setIcon(const TSoftObjectPtr<> &icon) {
    factory.setIcon(reinterpret_cast<const TSoftObjectPtr<UTexture2D>&>(icon));
}

void SDFCategory_Impl::setOrderingIndex(const int orderingIndex) {
    factory.setOrderingIndex(orderingIndex);
}

UObject *SDFCategory_Impl::createOrModifyCategory() {
    return factory.registerCategory();
}
