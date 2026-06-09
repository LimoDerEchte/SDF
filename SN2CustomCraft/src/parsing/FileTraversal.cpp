//
// Created by Limo on 23/05/2026.
//

#include "FileTraversal.hpp"

#include "util/Log.hpp"

namespace fs = std::filesystem;

std::vector<ScannedFile> FileTraversal::categoryTables;
std::vector<ScannedFile> FileTraversal::recipeTables;

static std::string trim(const std::string &text)
{
    const auto first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos)
        return {};

    const auto last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

void FileTraversal::ScanFiles() {
    Log::Verbose("Starting file traversal");

    std::ifstream mods("ue4ss\\Mods\\mods.txt");
    std::vector<std::string> enabledMods{};
    std::string line;

    while (std::getline(mods, line)) {
        const auto trimmed = trim(line);
        if (trimmed.starts_with(";") || trimmed.empty())
            continue;

        const auto colonPos = trimmed.find(":");
        std::string modName = trim(trimmed.substr(0, colonPos));

        if (std::string status = trim(trimmed.substr(colonPos + 1)); status == "1")
            enabledMods.push_back(modName);
    }

    for (const auto& mod : fs::directory_iterator("ue4ss\\Mods")) {
        if (!mod.is_directory())
            continue;

        std::string modName = mod.path().filename().string();
        if (!std::ranges::contains(enabledMods, modName) && !fs::exists(mod.path() / "enabled.txt"))
            continue;

        const auto path = mod.path() / "SDF";
        if (!fs::exists(path) || !fs::is_directory(path))
            continue;

        Log::Verbose("SDF folder found in mod {}", modName);
        for (const auto& file : fs::recursive_directory_iterator(path)) {
            if (!file.is_regular_file())
                continue;

            if (!file.path().filename().string().ends_with(".toml"))
                continue;

            try {
                toml::table toml = toml::parse_file(file.path().string());
                ScannedFile scan(modName, file.path().string(), toml);

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
