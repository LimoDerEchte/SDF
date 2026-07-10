//
// Created by Limo on 25/05/2026.
//

#pragma once

#include "sdk/Types.hpp"

class ItemTypeFactory {
    std::string itemId, itemName, itemDescription;
    TSoftObjectPtr<UTexture2D> itemTexture;

    TSoftClassPtr<UClass> actorClass;
    bool actorClassModified;

    bool modifyMode = false;
    bool itemTextureModified;

    friend class Hooks;

public:
    explicit ItemTypeFactory(std::string itemId, bool modifyMode);

    void setName(const std::string &itemName);
    void setDescription(const std::string &itemDescription);

    bool setActor(const std::string& actorPath);
    bool setActor(UClass* actorClass);
    void setActor(const TSoftClassPtr<UClass> &actorClass);

    bool setIconFromItem(const std::string &itemId);
    bool setIconFromItem(const UUWEItemType *item);
    bool setIcon(UTexture2D *icon);
    void setIcon(const TSoftObjectPtr<UTexture2D> &icon);

    [[nodiscard]] UUWEItemType* registerItemType() const;
    [[nodiscard]] static UUWEItemType* registerItemType(const std::string &itemId, const std::string &path);
};
