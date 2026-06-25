//
// Created by Limo on 25/05/2026.
//

#pragma once

#include "SDK/Subnautica2_classes.hpp"

class ItemTypeFactory {
    std::string itemId, itemName, itemDescription;
    SDK::TSoftObjectPtr<SDK::UTexture2D> itemTexture;

    SDK::TSoftClassPtr<SDK::UClass> actorClass;
    bool actorClassModified;

    bool modifyMode = false;
    bool itemTextureModified;

    friend class Hooks;

public:
    explicit ItemTypeFactory(std::string itemId, bool modifyMode);

    void setName(const std::string &itemName);
    void setDescription(const std::string &itemDescription);

    bool setActor(const std::string& actorPath);
    bool setActor(SDK::UClass* actorClass);
    void setActor(const SDK::TSoftClassPtr<SDK::UClass> &actorClass);

    bool setIconFromItem(const std::string &itemId);
    bool setIconFromItem(const SDK::UUWEItemType *item);
    bool setIcon(SDK::UTexture2D *icon);
    void setIcon(const SDK::TSoftObjectPtr<SDK::UTexture2D> &icon);

    [[nodiscard]] SDK::UUWEItemType* registerItemType() const;
    [[nodiscard]] static SDK::UUWEItemType* registerItemType(const std::string &itemId, const std::string &path);
};
