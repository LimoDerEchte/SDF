//
// Created by Limo on 23/05/2026.
//

#pragma once

#include <map>
#include "SDK/UWECrafting_classes.hpp"
#include "toml++/impl/table.hpp"

using namespace SDK;

class RecipeParser {
    static std::map<std::string, UUWECraftingRecipe*> recipies;

    static void parseFile(std::string file, const toml::table &table);

public:
    static void parseRecipes();
};
