//
// Created by Limo on 09/06/2026.
//

#include "parsing/IconParser.hpp"

#include <filesystem>

#include "toml++/impl/value.hpp"
#include "util/Finders.hpp"

namespace fs = std::filesystem;

using namespace SDK;

IconParser::IconParser(toml::node_view<const toml::node> node, const std::string& modName)
    : texture(), result(FailedUnexpected) {

    if (!node.is_string()) {
        result = InvalidToml;
        return;
    }
    auto content = node.as_string()->get();

    // Default Icon
    if (content == "DEFAULT") {
        const auto temp = UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(Finders::findCicadaTexture());
        texture = *reinterpret_cast<const TSoftObjectPtr<UTexture2D>*>(&temp);
        result = Success;
        return;
    }

    // Copy Item Icon
    if (content.starts_with("ITEM ")) {
        if (const auto item = Finders::searchItem(content.substr(5)); item == nullptr) {
            errorMessage = "Could not find item: " + content.substr(5);
            result = FailedMessage;
        } else {
            texture = item->Thumbnail;
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

        const auto worldContext = UWorld::GetWorld();
        if (const auto tex = UKismetRenderingLibrary::ImportFileAsTexture2D(worldContext, UtfN::StringToWString(absolute(actualPath).string()).c_str()); tex == nullptr) {
            errorMessage = "Could not read file: " + actualPath.string();
            result = FailedMessage;
        } else {
            const auto temp = UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(tex);
            texture = *reinterpret_cast<const TSoftObjectPtr<UTexture2D>*>(&temp);
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

TSoftObjectPtr<UTexture2D> IconParser::getTexture() const {
    return texture;
}
