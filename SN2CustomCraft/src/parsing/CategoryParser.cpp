//
// Created by Limo on 23/05/2026.
//

#include "CategoryParser.hpp"

#include "FileTraversal.hpp"
#include "registering/CategoryFactory.hpp"
#include "util/Log.hpp"

std::map<std::string, UUWECraftingRecipeCategory*> CategoryParser::categories{};

const std::map<std::string, ECrafterType> craftedByMap = {
    { "Fabricator", ECrafterType::Fabricator },
    { "ModificationStation", ECrafterType::ModificationStation },
    { "Processor", ECrafterType::Refinery },
    { "BuilderTool", ECrafterType::BuilderTool },
    { "VehicleFabricator", ECrafterType::VehicleFabricator }
};

void CategoryParser::parseFile(std::string file, const toml::table &table) {
    if (!table.contains("id") || !table["id"].is_string()) {
        Log::Warning("File {} has a category without an id", file);
        return;
    }
    const auto categoryId = table["id"].as_string()->get();

    if (!table.contains("name") || !table["name"].is_string()) {
        Log::Warning("Category {} is missing a name", categoryId);
        return;
    }
    const auto categoryName = table["name"].as_string()->get();

    if (!table.contains("description") || !table["description"].is_string()) {
        Log::Warning("Category {} is missing a description", categoryId);
        return;
    }
    const auto categoryDescription = table["description"].as_string()->get();

    if (!table.contains("crafted_by") || !table["crafted_by"].is_string()) {
        Log::Warning("Category {} is missing the crafted_by attribute", categoryId);
        return;
    }
    const auto craftedByString = table["crafted_by"].as_string()->get();

    if (!craftedByMap.contains(craftedByString)) {
        Log::Warning("Category {} has invalid value '{}' for crafted_by attribute", categoryId, craftedByString);
        return;
    }
    const auto craftedBy = craftedByMap.at(craftedByString);

    CategoryFactory factory(categoryId, categoryName, categoryDescription, craftedBy);

    if (table.contains("parent_category") && table["parent_category"].is_string()) {
        if (const auto parentCategory = table["parent_category"].as_string()->get(); !factory.setParent(parentCategory)) {
            Log::Warning("Category {} has invalid parent category '{}'", categoryId, parentCategory);
        }
    }

    if (table.contains("icon") && table["icon"].is_string()) {
        if (const auto iconPath = table["icon"].as_string()->get(); iconPath == "DEFAULT")
            factory.setIcon(nullptr);
        else if (iconPath.starts_with("ITEM ")) {
            if (!factory.setIconFromItem(iconPath.substr(5))) {
                Log::Warning("Category {} has invalid item icon path '{}'", categoryId, iconPath);
                return;
            }
        } else {
            Log::Warning("Category {} has invalid icon path '{}'", categoryId, iconPath);
            return;
        }
    }

    if (const auto result = factory.registerCategory(); result == nullptr)
        Log::Warning("Category {} failed to register for an unknown reason", categoryId);
    else
        categories.insert(std::make_pair(categoryId, result));
}

void CategoryParser::ParseCategories() {
    for (const auto&[path, toml] : FileTraversal::categoryTables) {
        if (!toml["category"].is_array()) {
            Log::Warning("Malformed category file {}", path);
            return;
        }

        for (const auto& category : *toml["category"].as_array()) {
            if (!category.is_table()) {
                Log::Warning("Malformed category file {}", path);
                return;
            }
            parseFile(path, *category.as_table());
        }
    }
}
