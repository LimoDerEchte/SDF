//
// Created by Limo on 23/05/2026.
//

#pragma once

#include <map>

#include "sdk/Types.hpp"
#include "toml++/impl/table.hpp"

class BuilderActionParser {
    static std::map<std::string, USN2BuilderConstructActionData*> builderActions;

    static void parseFile(const std::string &mod, const std::string &file, const toml::table &table, bool modifyMode);

public:
    static void ParseBuilderActions();
};
