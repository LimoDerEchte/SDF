//
// Created by Limo on 09/06/2026.
//

#pragma once

#include "SDK/Engine_classes.hpp"
#include "toml++/impl/node.hpp"
#include "toml++/impl/node_view.hpp"

enum IconParseResult {
    InvalidToml,
    FailedMessage,
    FailedUnexpected,
    Success
};

class IconParser {
    std::string errorMessage;
    SDK::UTexture2D *texture;
    IconParseResult result;

public:
    explicit IconParser(toml::node_view<const toml::node> node, const std::string& modName);

    IconParseResult getResult() const;
    std::string getErrorMessage() const;
    SDK::UTexture2D *getTexture() const;
};
