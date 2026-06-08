//
// Created by Limo on 23/05/2026.
//

#include "FileTraversal.hpp"

#include "util/Log.hpp"

namespace fs = std::filesystem;

std::vector<ScannedFile> FileTraversal::categoryTables;
std::vector<ScannedFile> FileTraversal::recipeTables;

void FileTraversal::ScanFiles() {
    Log::Verbose("Starting file traversal");

    for (const auto& mod : fs::directory_iterator("ue4ss\\Mods")) {
        if (!mod.is_directory())
            continue;

        const auto path = mod.path() / "CustomCraft";
        if (!fs::exists(path) || !fs::is_directory(path))
            continue;

        Log::Verbose("CustomCraft folder found in mod {}", mod.path().filename().string());
        for (const auto& file : fs::recursive_directory_iterator(path)) {
            if (!file.is_regular_file())
                continue;

            if (!file.path().filename().string().ends_with(".toml"))
                continue;

            try {
                toml::table toml = toml::parse_file(file.path().string());
                ScannedFile scan(file.path().string(), toml);

                if (toml.contains("category") || toml.contains("category_modify") || toml.contains("category_delete"))
                    categoryTables.push_back(scan);
                if (toml.contains("recipe") || toml.contains("recipe_modify") || toml.contains("recipe_delete"))
                    recipeTables.push_back(scan);
            } catch (const toml::parse_error& e) {
                Log::Warning("Failed to parse file {}: {}", file.path().string(), e.what());
            }
        }
    }
}

void FileTraversal::DeleteCache() {
    categoryTables.clear();
    recipeTables.clear();
}
