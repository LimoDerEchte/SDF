//
// Created by Limo on 23/05/2026.
//

#pragma once

#include "toml++/toml.hpp"

struct ScannedFile {
    std::string mod;
    std::string path;
    toml::table toml;
};

class FileTraversal {
    static std::vector<ScannedFile> categoryTables;
    static std::vector<ScannedFile> recipeTables;
    static std::vector<ScannedFile> storyGoalTables;
    static std::vector<ScannedFile> databankEntryTables;
    static std::vector<ScannedFile> itemTypeTables;
    static std::vector<ScannedFile> builderActionTables;

    friend class CategoryParser;
    friend class RecipeParser;
    friend class ItemTypeParser;
    friend class StoryGoalParser;
    friend class DatabankEntryParser;
    friend class BuilderActionParser;
    friend class SN2CustomCraft;

    static void ScanFiles();
    static void DeleteCache();
};
