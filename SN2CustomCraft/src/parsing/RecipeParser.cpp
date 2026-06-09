//
// Created by Limo on 23/05/2026.
//

#include "RecipeParser.hpp"

#include "FileTraversal.hpp"
#include "IconParser.hpp"
#include "registering/RecipeFactory.hpp"
#include "util/Log.hpp"

std::map<std::string, UUWECraftingRecipe*> RecipeParser::recipies{};

void RecipeParser::parseFile(const std::string &mod, const std::string &file, const toml::table &table, const bool modifyMode) {
    if (!table.contains("id") || !table["id"].is_string()) {
        Log::Warning("File {} has a recipe without an id", file);
        return;
    }
    const auto recipeId = table["id"].as_string()->get();

    if (!modifyMode && (!table.contains("name") || !table["name"].is_string())) {
        Log::Warning("Recipe {} is missing a name", recipeId);
        return;
    }
    const auto recipeName = !table.contains("name") ? "Empty" : table["name"].as_string()->get();

    if (!modifyMode && (!table.contains("description") || !table["description"].is_string())) {
        Log::Warning("Recipe {} is missing a description", recipeId);
        return;
    }
    const auto recipeDescription = !table.contains("description") ? "Empty" : table["description"].as_string()->get();

    if (!modifyMode && (!table.contains("category") || !table["category"].is_string())) {
        Log::Warning("Recipe {} is missing a category", recipeId);
    }

    auto factory = RecipeFactory(recipeId, modifyMode);
    if (recipeName != "Empty")
        factory.setName(recipeName);
    if (recipeDescription != "Empty")
        factory.setDescription(recipeDescription);

    if (!modifyMode || table.contains("category")) {
        if (const auto parentCategory = table["category"].as_string()->get(); !factory.setCategory(parentCategory)) {
            Log::Warning("Recipe {} has invalid category '{}'", recipeId, parentCategory);
            return;
        }
    }

    if (table.contains("icon")) {
        switch (const IconParser iconParser(table["icon"], mod); iconParser.getResult()) {
            case InvalidToml:
                Log::Warning("Recipe {} has malformed icon", recipeId);
                break;
            case FailedMessage:
                Log::Warning("Recipe {} failed to read icon: {}", recipeId, iconParser.getErrorMessage());
                break;
            case FailedUnexpected:
                Log::Warning("Recipe {} failed to read icon unexpectedly", recipeId);
                break;
            case Success:
                factory.setIcon(iconParser.getTexture());
                break;
        }
    }

    if (table.contains("ingredients") && table["ingredients"].is_array() && table["ingredients"].as_array()->size() > 0) {
        for (const auto& ingredientTable : *table["ingredients"].as_array()) {
            if (!ingredientTable.is_table()) {
                Log::Warning("Recipe {} has malformed entry for ingredients", recipeId);
                return;
            }
            for (const auto&[first, second] : *ingredientTable.as_table()) {
                if (!second.is_integer()) {
                    Log::Warning("Recipe {} has malformed amount for ingredient {}", recipeId, first.str());
                    return;
                }
                if (const auto count = static_cast<int32_t>(second.as_integer()->get());
                    !factory.addIngredient(std::string(first.begin(), first.end()), count))
                    Log::Warning("Failed to add ingredient {} to recipe {}", first.str(), recipeId);
            }
        }
    } else if (!modifyMode)
        Log::Warning("Recipe {} has no ingredients", recipeId);

    if (table.contains("outputs") && table["outputs"].is_array() && table["outputs"].as_array()->size() > 0) {
        for (const auto& outputTable : *table["outputs"].as_array()) {
            if (!outputTable.is_table()) {
                Log::Warning("Recipe {} has malformed entry for outputs", recipeId);
                return;
            }
            for (const auto&[first, second] : *outputTable.as_table()) {
                if (!second.is_integer()) {
                    Log::Warning("Recipe {} has malformed amount for output {}", recipeId, first.str());
                    return;
                }
                if (const auto count = static_cast<int32_t>(second.as_integer()->get());
                    !factory.addOutput(std::string(first.begin(), first.end()), count))
                    Log::Warning("Failed to add output {} to recipe {}", first.str(), recipeId);
            }
        }
    } else if (!modifyMode)
        Log::Warning("Recipe {} has no outputs", recipeId);

    if (table.contains("unlocking_rule") && table["unlocking_rule"].is_array() && table["unlocking_rule"].as_array()->size() > 0) {
        for (const auto& unlockingRuleTableNode : *table["unlocking_rule"].as_array()) {
            if (!unlockingRuleTableNode.is_table()) {
                Log::Warning("Recipe {} has malformed entry for unlocking rules", recipeId);
                return;
            }
            const auto unlockingRuleTable = unlockingRuleTableNode.as_table();
            if (!unlockingRuleTable->get("set_id")->is_string()) {
                Log::Warning("Recipe {} is missing set_id for unlocking rule entry", recipeId);
                return;
            }

            const auto ruleSetId = unlockingRuleTable->get("set_id")->as_string()->get();
            auto anyRulePresent = false;

            if (unlockingRuleTable->contains("pickup_items") && unlockingRuleTable->get("pickup_items")->is_array()) {
                for (const auto& pickupRule : *unlockingRuleTable->get("pickup_items")->as_array()) {
                    if (!pickupRule.is_string()) {
                        Log::Warning("Recipe {} has malformed item pickup rule in rule set {}", recipeId, ruleSetId);
                        return;
                    }
                    if (!factory.addUnlockingRequirementPickup(ruleSetId, pickupRule.as_string()->get()))
                        Log::Warning("Failed to add item pickup rule to rule set {} of recipe {}", ruleSetId, recipeId);
                    else
                        anyRulePresent = true;
                }
            }

            if (unlockingRuleTable->contains("scan_data") && unlockingRuleTable->get("scan_data")->is_array()) {
                for (const auto& scanDataRule : *unlockingRuleTable->get("scan_data")->as_array()) {
                    if (!scanDataRule.is_string()) {
                        Log::Warning("Recipe {} has malformed scan data rule in rule set {}", recipeId, ruleSetId);
                        return;
                    }
                    if (!factory.addUnlockingRequirementScanData(ruleSetId, scanDataRule.as_string()->get()))
                        Log::Warning("Failed to add scan data rule to rule set {} of recipe {}", ruleSetId, recipeId);
                    else
                        anyRulePresent = true;
                }
            }

            if (!anyRulePresent)
                Log::Warning("Recipe {} has no unlocking rules in rule set {}", recipeId, ruleSetId);
        }
    }

    if (table.contains("crafting_time") && table["crafting_time"].is_number()) {
        const auto time = static_cast<float>(table["crafting_time"].as_floating_point()->get());
        factory.setCraftingTime(time);
    }

    if (table.contains("available_in_lifepod") && table["available_in_lifepod"].is_boolean()) {
        factory.setAvailableInLifepod(table["available_in_lifepod"].as_boolean()->get());
    }

    if (table.contains("remove_unlocking_rules") && table["remove_unlocking_rules"].is_boolean() && table["remove_unlocking_rules"].as_boolean()->get()) {
        factory.modifyRemoveRequirements();
    }

    if (table.contains("ordering_index") && table["ordering_index"].is_integer()) {
        factory.setOrderingIndex(static_cast<int32_t>(table["ordering_index"].as_integer()->get()));
    }

    if (const auto result = factory.registerRecipe(); result == nullptr)
        Log::Warning("Recipe {} failed to {} for an unknown reason", modifyMode ? "modify" : "register", recipeId);
    else
        recipies.insert(std::make_pair(recipeId, result));
}

void RecipeParser::ParseRecipes() {
    for (const auto&[mod, path, toml] : FileTraversal::recipeTables) {
        if (toml.contains("recipe")) {
            if (!toml["recipe"].is_array()) {
                Log::Warning("Malformed recipe file {}", path);
                return;
            }

            for (const auto& recipe : *toml["recipe"].as_array()) {
                if (!recipe.is_table()) {
                    Log::Warning("Malformed recipe file {}", path);
                    return;
                }
                parseFile(mod, path, *recipe.as_table(), false);
            }
        }

        if (toml.contains("recipe_modify")) {
            if (!toml["recipe_modify"].is_array()) {
                Log::Warning("Malformed recipe file {}", path);
                return;
            }

            for (const auto& recipe : *toml["recipe_modify"].as_array()) {
                if (!recipe.is_table()) {
                    Log::Warning("Malformed recipe file {}", path);
                    return;
                }
                parseFile(mod, path, *recipe.as_table(), true);
            }
        }

        if (toml.contains("recipe_delete")) {
            if (!toml["recipe_delete"].is_array()) {
                Log::Warning("Malformed recipe file {}", path);
                return;
            }

            for (const auto& recipe : *toml["recipe_delete"].as_array()) {
                if (!recipe.is_table()) {
                    Log::Warning("Malformed recipe file {}", path);
                    return;
                }
                const auto table = *recipe.as_table();

                if (!table.contains("id") || !table["id"].is_string()) {
                    Log::Warning("File {} has a recipe_delete without an id", path);
                    return;
                }
                const auto recipeId = table["id"].as_string()->get();

                RecipeFactory factory(recipeId, true);
                factory.setCategory("Fabricator");
                const auto _ = factory.registerRecipe();
            }
        }
    }
}
