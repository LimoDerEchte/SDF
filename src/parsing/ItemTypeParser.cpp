//
// Created by Limo on 23/05/2026.
//

#include "parsing/ItemTypeParser.hpp"

#include "parsing/FileTraversal.hpp"
#include "generic/IconParser.hpp"
#include "registering/ItemTypeFactory.hpp"
#include "util/Log.hpp"

std::map<std::string, UUWEItemType*> ItemTypeParser::itemTypes{};


void ItemTypeParser::parseFile(const std::string &mod, const std::string &file, const toml::table &table, const bool modifyMode) {
    if (!table.contains("id") || !table["id"].is_string()) {
        Log::Warning("File {} has an item type without an id", file);
        return;
    }
    const auto itemId = table["id"].as_string()->get();

    if (!modifyMode && (!table.contains("name") || !table["name"].is_string())) {
        Log::Warning("Item type {} is missing a name", itemId);
        return;
    }
    const auto itemName = table.contains("name") ? table["name"].as_string()->get() : "Empty";

    if (!modifyMode && (!table.contains("description") || !table["description"].is_string())) {
        Log::Warning("Item type {} is missing a description", itemId);
        return;
    }
    const auto itemDescription = table.contains("description") ? table["description"].as_string()->get() : "Empty";

    ItemTypeFactory factory(itemId, modifyMode);
    if (itemName != "Empty")
        factory.setName(itemName);
    if (itemDescription != "Empty")
        factory.setDescription(itemDescription);

    if (table.contains("icon")) {
        switch (const IconParser iconParser(table["icon"], mod); iconParser.getResult()) {
            case InvalidToml:
                Log::Warning("Item type {} has malformed icon", itemId);
                break;
            case FailedMessage:
                Log::Warning("Item type {} failed to read icon: {}", itemId, iconParser.getErrorMessage());
                break;
            case FailedUnexpected:
                Log::Warning("Item type {} failed to read icon unexpectedly", itemId);
                break;
            case Success:
                factory.setIcon(iconParser.getTexture());
                break;
        }
    }

    if (table.contains("actor_path")) {
        if (!table["actor_path"].is_string()) {
            Log::Warning("Item type {} has malformed actor path", itemId);
            return;
        }
        const auto actorPath = table["actor_path"].as_string()->get();
        //factory.setActor(actorPath);
    }

    if (const auto result = factory.registerItemType(); result == nullptr)
        Log::Warning("Item type {} failed to {} for an unknown reason", modifyMode ? "modify" : "register", itemId);
    else
        itemTypes.insert(std::make_pair(itemId, result));
}

void ItemTypeParser::ParseItemTypes() {
    for (const auto&[mod, path, toml] : FileTraversal::itemTypeTables) {
        if (toml.contains("item_type_register")) {
            if (!toml["item_type_register"].is_array()) {
                Log::Warning("Malformed item type file {}", path);
                return;
            }

            for (const auto& itemType : *toml["item_type_register"].as_array()) {
                if (!itemType.is_table()) {
                    Log::Warning("Malformed item type file {}", path);
                    return;
                }
                const auto table = *itemType.as_table();

                if (!table.contains("id") || !table["id"].is_string()) {
                    Log::Warning("File {} has an item type registration without an id", path);
                    return;
                }
                const auto itemId = table["id"].as_string()->get();

                if (!table.contains("path") || !table["path"].is_string()) {
                    Log::Warning("Item type {} is missing an asset path", itemId);
                    return;
                }
                const auto assetPath = table["path"].as_string()->get();

                if (const auto result = ItemTypeFactory::registerItemType(itemId, assetPath); result == nullptr)
                    Log::Warning("Item type {} failed to register for an unknown reason", itemId);
                else
                    itemTypes.insert(std::make_pair(itemId, result));
            }
        }

        if (toml.contains("item_type")) {
            if (!toml["item_type"].is_array()) {
                Log::Warning("Malformed item type file {}", path);
                return;
            }

            for (const auto& itemType : *toml["item_type"].as_array()) {
                if (!itemType.is_table()) {
                    Log::Warning("Malformed item type file {}", path);
                    return;
                }
                parseFile(mod, path, *itemType.as_table(), false);
            }
        }

        if (toml.contains("item_type_modify")) {
            if (!toml["item_type_modify"].is_array()) {
                Log::Warning("Malformed item type file {}", path);
                return;
            }

            for (const auto& itemType : *toml["item_type_modify"].as_array()) {
                if (!itemType.is_table()) {
                    Log::Warning("Malformed item type file {}", path);
                    return;
                }
                parseFile(mod, path, *itemType.as_table(), true);
            }
        }
    }
}
