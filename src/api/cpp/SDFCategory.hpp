//
// Created by Limo on 28/06/2026.
//

#pragma once

#include "registering/CategoryFactory.hpp"
#include "SDF/SDFCategory.hpp"

class SDFCategory_Impl final : public SDFCategory {
    CategoryFactory factory;

public:
    explicit SDFCategory_Impl(const std::string &id, bool modifyMode);

    void setName(const std::string &name) override;
    [[deprecated]] void setDescription(const std::string &description) override;
    void setCrafterType(uint8_t type) override;

    void setParent(const std::string &category) override;
    void setParent(RC::Unreal::UObject *category) override;

    void setIcon(const std::string &icon) override;
    void setIcon(RC::Unreal::UObject *icon) override;
    void setIcon(const RC::Unreal::TSoftObjectPtr<> &icon) override;

    void setOrderingIndex(int orderingIndex) override;

    [[nodiscard]] RC::Unreal::UObject *createOrModifyCategory() override;
};
