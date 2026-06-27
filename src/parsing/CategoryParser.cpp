//
// Created by Limo on 23/05/2026.
//

#include "parsing/CategoryParser.hpp"

#include "parsing/FileTraversal.hpp"
#include "generic/IconParser.hpp"
#include "registering/CategoryFactory.hpp"
#include "util/Log.hpp"

using namespace SDK;

std::map<std::string, UUWECraftingRecipeCategory*> CategoryParser::categories{};

const std::map<std::string, ECrafterType> craftedByMap = {
    { "Fabricator", ECrafterType::Fabricator },
    { "ModificationStation", ECrafterType::ModificationStation },
    { "Processor", ECrafterType::Refinery },
    { "BuilderTool", ECrafterType::BuilderTool },
    { "VehicleFabricator", ECrafterType::VehicleFabricator }
};

void CategoryParser::parseFile(const std::string &mod, const std::string &file, const toml::table &table, const bool modifyMode) {
    if (!table.contains("id") || !table["id"].is_string()) {
        Log::Warning("File {} has a category without an id", file);
        return;
    }
    const auto categoryId = table["id"].as_string()->get();

    if (!modifyMode && (!table.contains("name") || !table["name"].is_string())) {
        Log::Warning("Category {} is missing a name", categoryId);
        return;
    }
    const auto categoryName = table.contains("name") ? table["name"].as_string()->get() : "Empty";

    if (!modifyMode && (!table.contains("description") || !table["description"].is_string())) {
        //Log::Warning("Category {} is missing a description", categoryId);
        //return;
        // Category descriptions are not actually needed at the current moment
    }
    const auto categoryDescription = table.contains("description") ? table["description"].as_string()->get() : "Empty";

    if (!modifyMode && (!table.contains("crafted_by") || !table["crafted_by"].is_string())) {
        Log::Warning("Category {} is missing the crafted_by attribute", categoryId);
        return;
    }
    const auto craftedByString = table.contains("crafted_by") ? table["crafted_by"].as_string()->get() : "Empty";

    if (craftedByString != "Empty" && !craftedByMap.contains(craftedByString)) {
        Log::Warning("Category {} has invalid value '{}' for crafted_by attribute", categoryId, craftedByString);
        return;
    }

    CategoryFactory factory(categoryId, modifyMode);
    if (categoryName != "Empty")
        factory.setName(categoryName);
    if (categoryDescription != "Empty")
        factory.setDescription(categoryDescription);
    if (craftedByString != "Empty")
        factory.setCrafterType(craftedByMap.at(craftedByString));

    if (table.contains("parent_category") && table["parent_category"].is_string()) {
        if (const auto parentCategory = table["parent_category"].as_string()->get(); !factory.setParent(parentCategory)) {
            Log::Warning("Category {} has invalid parent category '{}'", categoryId, parentCategory);
        }
    }

    if (table.contains("icon")) {
        switch (const IconParser iconParser(table["icon"], mod); iconParser.getResult()) {
            case InvalidToml:
                Log::Warning("Category {} has malformed icon", categoryId);
                break;
            case FailedMessage:
                Log::Warning("Category {} failed to read icon: {}", categoryId, iconParser.getErrorMessage());
                break;
            case FailedUnexpected:
                Log::Warning("Category {} failed to read icon unexpectedly", categoryId);
                break;
            case Success:
                factory.setIcon(iconParser.getTexture());
                break;
        }
    }

    if (table.contains("ordering_index") && table["ordering_index"].is_integer()) {
        factory.setOrderingIndex(static_cast<int32_t>(table["ordering_index"].as_integer()->get()));
    }

    if (table.contains("show_when_empty") && table["show_when_empty"].is_boolean()) {
        factory.setShowWhenEmpty(table["show_when_empty"].as_boolean()->get());
    }

    if (const auto result = factory.registerCategory(); result == nullptr)
        Log::Warning("Category {} failed to {} for an unknown reason", modifyMode ? "modify" : "register", categoryId);
    else
        categories.insert(std::make_pair(categoryId, result));
}

void CategoryParser::ParseCategories() {
    for (const auto&[mod, path, toml] : FileTraversal::categoryTables) {
        if (toml.contains("category")) {
            if (!toml["category"].is_array()) {
                Log::Warning("Malformed category file {}", path);
                return;
            }

            for (const auto& category : *toml["category"].as_array()) {
                if (!category.is_table()) {
                    Log::Warning("Malformed category file {}", path);
                    return;
                }
                parseFile(mod, path, *category.as_table(), false);
            }
        }

        if (toml.contains("category_modify")) {
            if (!toml["category_modify"].is_array()) {
                Log::Warning("Malformed category file {}", path);
                return;
            }

            for (const auto& category : *toml["category_modify"].as_array()) {
                if (!category.is_table()) {
                    Log::Warning("Malformed category file {}", path);
                    return;
                }
                parseFile(mod, path, *category.as_table(), true);
            }
        }

        if (toml.contains("category_delete")) {
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

                CategoryFactory factory(recipeId, true);
                const auto cat = factory.registerCategory();
                cat->ParentCategory = static_cast<TSoftObjectPtr<UUWECraftingRecipeCategory>>(UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(nullptr));
            }
        }
    }
}
