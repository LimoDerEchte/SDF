//
// Created by Limo on 23/05/2026.
//

#pragma once

#include <map>

#include "sdk/Types.hpp"
#include "toml++/impl/table.hpp"

class DatabankEntryParser {
    static std::map<std::string, UUWEDatabankEntry*> databankEntries;

    static void parseFile(const std::string &mod, const std::string &file, const toml::table &table, bool modifyMode);

public:
    static void ParseDatabankEntries();
};
