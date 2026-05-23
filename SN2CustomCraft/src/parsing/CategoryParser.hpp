//
// Created by Limo on 23/05/2026.
//

#pragma once

#include <map>
#include "SDK/UWECrafting_classes.hpp"
#include "toml++/impl/table.hpp"

using namespace SDK;

class CategoryParser {
    static std::map<std::string, UUWECraftingRecipeCategory*> categories;

    static void parseFile(std::string file, const toml::table &table);

public:
    static void ParseCategories();
};
