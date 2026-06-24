//
// Created by Limo on 23/05/2026.
//

#pragma once

#include <map>

#include "SDK/UWEDatabank_classes.hpp"
#include "toml++/impl/table.hpp"

class DatabankEntryParser {
    static std::map<std::string, SDK::UUWEDatabankEntry*> databankEntries;

    static void parseFile(const std::string &mod, const std::string &file, const toml::table &table, bool modifyMode);

public:
    static void ParseDatabankEntries();
};
