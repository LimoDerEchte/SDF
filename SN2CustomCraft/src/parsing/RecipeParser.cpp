//
// Created by Limo on 23/05/2026.
//

#include "RecipeParser.hpp"

#include "FileTraversal.hpp"
#include "registering/RecipeFactory.hpp"
#include "util/Log.hpp"

std::map<std::string, UUWECraftingRecipe*> RecipeParser::recipies{};

void RecipeParser::parseFile(std::string file, const toml::table &table) {
    if (!table.contains("id") || !table["id"].is_string()) {
        Log::Warning("File {} has a recipe without an id", file);
        return;
    }
    const auto recipeId = table["id"].as_string()->get();

    if (!table.contains("name") || !table["name"].is_string()) {
        Log::Warning("Recipe {} is missing a name", recipeId);
        return;
    }
    const auto recipeName = table["name"].as_string()->get();

    if (!table.contains("description") || !table["description"].is_string()) {
        Log::Warning("Recipe {} is missing a description", recipeId);
        return;
    }
    const auto recipeDescription = table["description"].as_string()->get();

    if (!table.contains("category") || !table["category"].is_string()) {
        Log::Warning("Recipe {} is missing a category", recipeId);
    }

    RecipeFactory factory(recipeId, recipeName, recipeDescription);

    if (const auto parentCategory = table["category"].as_string()->get(); !factory.setCategory(parentCategory)) {
        Log::Warning("Recipe {} has invalid category '{}'", recipeId, parentCategory);
        return;
    }

    if (table.contains("icon") && table["icon"].is_string()) {
        if (const auto iconPath = table["icon"].as_string()->get(); iconPath == "DEFAULT")
            factory.setIcon(nullptr);
        else if (iconPath.starts_with("ITEM ")) {
            if (!factory.setIconFromItem(iconPath.substr(5))) {
                Log::Warning("Recipe {} has invalid item icon path '{}'", recipeId, iconPath);
                return;
            }
        } else {
            Log::Warning("Recipe {} has invalid icon path '{}'", recipeId, iconPath);
            return;
        }
    }

    if (table.contains("ingredients") && table["ingredients"].is_table()) {
        for (const auto&[first, second] : *table["ingredients"].as_table()) {
            if (!second.is_integer()) {
                Log::Warning("Recipe {} has malformed amount for ingredient {}", recipeId, first.str());
                return;
            }
            if (const auto count = static_cast<int32_t>(second.as_integer()->get());
                !factory.addIngredient(std::string(first.begin(), first.end()), count))
                Log::Warning("Failed to add ingredient {} to recipe {}", first.str(), recipeId);
        }
    } else
        Log::Warning("Recipe {} has no ingredients", recipeId);

    if (table.contains("outputs") && table["outputs"].is_table()) {
        for (const auto&[first, second] : *table["outputs"].as_table()) {
            if (!second.is_integer()) {
                Log::Warning("Recipe {} has malformed amount for output {}", recipeId, first.str());
                return;
            }
            if (const auto count = static_cast<int32_t>(second.as_integer()->get());
                !factory.addOutput(std::string(first.begin(), first.end()), count))
                Log::Warning("Failed to add output {} to recipe {}", first.str(), recipeId);
        }
    } else
        Log::Warning("Recipe {} has no outputs", recipeId);

    if (table.contains("crafting_time") && table["crafting_time"].is_number()) {
        const auto time = static_cast<float>(table["crafting_time"].as_floating_point()->get());
        factory.setCraftingTime(time);
    } else
        Log::Warning("Recipe {} has no crafting time", recipeId);

    if (const auto result = factory.registerRecipe(); result == nullptr)
        Log::Warning("Recipe {} failed to register for an unknown reason", recipeId);
    else
        recipies.insert(std::make_pair(recipeId, result));
}

void RecipeParser::parseRecipes() {
    for (const auto&[path, toml] : FileTraversal::recipeTables) {
        if (!toml["recipe"].is_array()) {
            Log::Warning("Malformed recipe file {}", path);
            return;
        }

        for (const auto& recipe : *toml["recipe"].as_array()) {
            if (!recipe.is_table()) {
                Log::Warning("Malformed recipe file {}", path);
                return;
            }
            parseFile(path, *recipe.as_table());
        }
    }
}
