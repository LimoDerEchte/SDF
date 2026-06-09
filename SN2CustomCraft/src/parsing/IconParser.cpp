//
// Created by Limo on 09/06/2026.
//

#include "IconParser.hpp"

#include <filesystem>

#include "toml++/impl/value.hpp"
#include "util/Finders.hpp"

namespace fs = std::filesystem;

IconParser::IconParser(toml::node_view<const toml::node> node, const std::string& modName)
    : errorMessage(""), texture(nullptr), result(FailedUnexpected) {

    if (!node.is_string()) {
        result = InvalidToml;
        return;
    }
    auto content = node.as_string()->get();

    // Default Icon
    if (content == "DEFAULT") {
        texture = Finders::findCicadaTexture();
        result = Success;
        return;
    }

    // Copy Item Icon
    if (content.starts_with("ITEM ")) {
        if (const auto item = Finders::searchItem(content.substr(5)); item == nullptr || item->Thumbnail.Get() == nullptr) {
            errorMessage = "Could not find item: " + content.substr(5);
            result = FailedMessage;
        } else {
            texture = item->Thumbnail.Get();
            result = Success;
        }
        return;
    }

    // Load From File
    if (content.starts_with("FILE ")) {
        fs::path actualPath("ue4ss");
        content = content.substr(5);

        if (content.starts_with("MOD ")) {
            content = content.substr(4);
            actualPath = actualPath / "Mods" / modName;
        } else if (content.starts_with("SDF ")) {
            content = content.substr(4);
            actualPath = actualPath / "Mods" / modName / "SDF";
        } else if (content.starts_with("UE4SS ")) {
            content = content.substr(6);
        } else {
            errorMessage = "File has invalid base location identifier. Available: MOD, SDF, UE4SS";
            result = FailedMessage;
            return;
        }

        actualPath = actualPath / content;
        if (!fs::exists(actualPath)) {
            errorMessage = "Could not find file: " + actualPath.string();
            result = FailedMessage;
            return;
        }

        const auto worldContext = SDK::UWorld::GetWorld();
        if (const auto tex = SDK::UKismetRenderingLibrary::ImportFileAsTexture2D(worldContext, UtfN::StringToWString(absolute(actualPath).string()).c_str()); tex == nullptr) {
            errorMessage = "Could not read file: " + actualPath.string();
            result = FailedMessage;
        } else {
            texture = tex;
            result = Success;
        }
        return;
    }

    errorMessage = "Invalid path: " + content;
    result = FailedMessage;
}

IconParseResult IconParser::getResult() const {
    return result;
}

std::string IconParser::getErrorMessage() const {
    return errorMessage;
}

SDK::UTexture2D *IconParser::getTexture() const {
    return texture;
}
