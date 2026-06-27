//
// Created by Limo on 23/05/2026.
//

#include "parsing/BuilderActionParser.hpp"

#include "parsing/FileTraversal.hpp"
#include "generic/IconParser.hpp"
#include "registering/BuilderActionFactory.hpp"
#include "util/Log.hpp"

using namespace SDK;

std::map<std::string, USN2BuilderConstructActionData*> BuilderActionParser::builderActions{};

void BuilderActionParser::parseFile(const std::string &mod, const std::string &file, const toml::table &table, const bool modifyMode) {
    if (!table.contains("id") || !table["id"].is_string()) {
        Log::Warning("File {} has an builder action without an id", file);
        return;
    }
    const auto actionId = table["id"].as_string()->get();

    if (!modifyMode && (!table.contains("recipe") || !table["recipe"].is_string())) {
        Log::Warning("Builder action {} is missing a recipe", actionId);
        return;
    }
    const auto actionRecipe = table.contains("recipe") ? table["recipe"].as_string()->get() : "Empty";

    BuilderActionFactory factory(actionId, modifyMode);
    if (actionRecipe != "Empty" && !factory.setRecipe(actionRecipe)) {
        Log::Warning("Failed to find recipe {} for builder action {}", actionRecipe, actionId);
        return;
    }

    if (table.contains("secondary_description") && table["secondary_description"].is_string())
        factory.setSecondaryDescription(table["secondary_description"].as_string()->get());

    if (table.contains("power_drain") && table["power_drain"].is_string())
        factory.addPowerDrainText(table["power_drain"].as_string()->get());
    if (table.contains("power_generation") && table["power_gain"].is_string())
        factory.addPowerGenerationText(table["power_generation"].as_string()->get());
    if (table.contains("remove_power_text") && table["remove_power_text"].is_boolean() && table["remove_power_text"].as_boolean()->get())
        factory.setRemovePowerText(true);

    if (const auto result = factory.registerBuilderAction(); result == nullptr)
        Log::Warning("Builder action {} failed to {} for an unknown reason", modifyMode ? "modify" : "register", actionId);
    else
        builderActions.insert(std::make_pair(actionId, result));
}

void BuilderActionParser::ParseBuilderActions() {
    for (const auto&[mod, path, toml] : FileTraversal::builderActionTables) {
        if (toml.contains("builder_action")) {
            if (!toml["builder_action"].is_array()) {
                Log::Warning("Malformed builder action file {} (builder_action)", path);
                return;
            }

            for (const auto& builderAction : *toml["builder_action"].as_array()) {
                if (!builderAction.is_table()) {
                    Log::Warning("Malformed builder action file {} (builder_action entry)", path);
                    return;
                }
                parseFile(mod, path, *builderAction.as_table(), false);
            }
        }

        if (toml.contains("builder_action_modify")) {
            if (!toml["builder_action_modify"].is_array()) {
                Log::Warning("Malformed builder action file {} (builder_action_modify)", path);
                return;
            }

            for (const auto& builderAction : *toml["builder_action_modify"].as_array()) {
                if (!builderAction.is_table()) {
                    Log::Warning("Malformed builder action file {} (builder_action_modify entry)", path);
                    return;
                }
                parseFile(mod, path, *builderAction.as_table(), true);
            }
        }
    }
}
