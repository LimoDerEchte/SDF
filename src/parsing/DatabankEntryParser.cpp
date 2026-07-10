//
// Created by Limo on 23/05/2026.
//

#include "parsing/DatabankEntryParser.hpp"

#include "parsing/FileTraversal.hpp"
#include "generic/IconParser.hpp"
#include "generic/StoryGoalRuleParser.hpp"
#include "registering/DatabankEntryFactory.hpp"
#include "util/Log.hpp"

std::map<std::string, UUWEDatabankEntry*> DatabankEntryParser::databankEntries{};

void DatabankEntryParser::parseFile(const std::string &mod, const std::string &file, const toml::table &table, const bool modifyMode) {
    if (!table.contains("id") || !table["id"].is_string()) {
        Log::Warning("File {} has a databank entry without an id", file);
        return;
    }
    const auto id = table["id"].as_string()->get();

    if (!modifyMode && (!table.contains("title") || !table["title"].is_string())) {
        Log::Warning("DatabankEntry {} is missing a title", id);
        return;
    }
    const auto title = table.contains("title") ? table["title"].as_string()->get() : "Empty";

    if (!modifyMode && (!table.contains("text") || !table["text"].is_string())) {
        Log::Warning("Databank entry {} is missing a text", id);
    }
    const auto text = table.contains("text") ? table["text"].as_string()->get() : "Empty";

    DatabankEntryFactory factory(id, modifyMode);
    if (title != "Empty")
        factory.setTitle(title);
    if (text != "Empty")
        factory.setText(text);

    if (table.contains("icon")) {
        switch (const IconParser iconParser(table["icon"], mod); iconParser.getResult()) {
            case InvalidToml:
                Log::Warning("Databank entry {} has malformed icon", id);
                break;
            case FailedMessage:
                Log::Warning("Databank entry {} failed to read icon: {}", id, iconParser.getErrorMessage());
                break;
            case FailedUnexpected:
                Log::Warning("Databank entry {} failed to read icon unexpectedly", id);
                break;
            case Success:
                factory.setIcon(iconParser.getTexture());
                break;
        }
    }

    if (table.contains("categories") && table["categories"].is_array()) {
        for (const auto& entry : *table["categories"].as_array()) {
            if (!entry.is_string()) {
                Log::Warning("Malformed databank entry file {}", file);
                return;
            }
            factory.addCategory(entry.as_string()->get());
        }
    }

    if (table.contains("unlocked_by") && table["unlocked_by"].is_string()) {
        const auto parsed = StoryGoalRuleParser::ParseRule(id, table["unlocked_by"].as_string()->get());
        if (parsed == nullptr) {
            Log::Warning("Failed to parse unlocked_by for story goal {}", id);
            return;
        }
        const auto rule = StoryGoalRuleFactory::registerStoryGoalRule(parsed);
        if (rule == nullptr) {
            Log::Warning("Failed to register story goal rule for story goal {}", id);
            return;
        }
        factory.setUnlockCondition(rule);
    }

    if (table.contains("hidden_by") && table["hidden_by"].is_string() && !factory.setHideCondition(table["hidden_by"].as_string()->get())) {
        Log::Warning("Failed to set hidden_by for story goal {}", id);
        return;
    }

    if (const auto result = factory.registerDatabankEntry(); result == nullptr)
        Log::Warning("Databank entry {} failed to {} for an unknown reason", modifyMode ? "modify" : "register", id);
    else
        databankEntries.insert(std::make_pair(id, result));
}

void DatabankEntryParser::ParseDatabankEntries() {
    for (const auto&[mod, path, toml] : FileTraversal::databankEntryTables) {
        if (toml.contains("databank_entry")) {
            if (!toml["databank_entry"].is_array()) {
                Log::Warning("Malformed databank entry file {}", path);
                return;
            }

            for (const auto& entry : *toml["databank_entry"].as_array()) {
                if (!entry.is_table()) {
                    Log::Warning("Malformed databank entry file {}", path);
                    return;
                }
                parseFile(mod, path, *entry.as_table(), false);
            }
        }

        if (toml.contains("databank_entry_modify")) {
            if (!toml["databank_entry_modify"].is_array()) {
                Log::Warning("Malformed databank entry file {}", path);
                return;
            }

            for (const auto& entry : *toml["databank_entry_modify"].as_array()) {
                if (!entry.is_table()) {
                    Log::Warning("Malformed databank entry file {}", path);
                    return;
                }
                parseFile(mod, path, *entry.as_table(), true);
            }
        }

        /*
        if (toml.contains("databank_entry_delete")) {
            if (!toml["category_delete"].is_array()) {
                Log::Warning("Malformed category file {}", path);
                return;
            }

            for (const auto& category : *toml["category_delete"].as_array()) {
                if (!category.is_table()) {
                    Log::Warning("Malformed category file {}", path);
                    return;
                }
                const auto table = *category.as_table();

                if (!table.contains("id") || !table["id"].is_string()) {
                    Log::Warning("File {} has a category_delete without an id", path);
                    return;
                }
                const auto recipeId = table["id"].as_string()->get();

                DatabankEntryFactory factory(recipeId, true);
                const auto cat = factory.registerDatabankEntry();
                cat->ParentDatabankEntry = static_cast<TSoftObjectPtr<UUWECraftingRecipeDatabankEntry>>(UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(nullptr));
            }
        }*/
    }
}
