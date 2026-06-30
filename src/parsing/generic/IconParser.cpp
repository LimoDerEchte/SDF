//
// Created by Limo on 09/06/2026.
//

#include "IconParser.hpp"

#include <filesystem>

#include "toml++/impl/node_view.hpp"
#include "toml++/impl/value.hpp"
#include "util/Finders.hpp"
#include "UnrealDef.hpp"

namespace fs = std::filesystem;

using namespace SDK;

void IconParser::parseInternal(std::string content, const std::string &modName) {
    // Default Icon
    if (content == "DEFAULT") {
        const auto temp = UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(Finders::searchTexture("T_DefaultImage"));
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

    if (content.starts_with("PATH ")) {
        if (const auto ptr = RC::UObjectGlobals::StaticFindObject(nullptr, nullptr, UtfN::StringToWString(content.substr(5)).c_str()); ptr == nullptr) {
            errorMessage = "Could not find texture: " + content.substr(5);
            result = FailedMessage;
        } else {
            const auto temp = UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(reinterpret_cast<UObject*>(ptr));
            texture = *reinterpret_cast<const TSoftObjectPtr<UTexture2D>*>(&temp);
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

IconParser::IconParser(const toml::node_view<const toml::node> node, const std::string& modName)
    : texture(), result(FailedUnexpected) {

    if (!node.is_string()) {
        result = InvalidToml;
        return;
    }
    parseInternal(node.as_string()->get(), modName);
}

IconParser::IconParser(const std::string &content, const std::string &modName) {
    parseInternal(content, modName);
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
