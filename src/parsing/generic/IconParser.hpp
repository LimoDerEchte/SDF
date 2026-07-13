//
// Created by Limo on 09/06/2026.
//

#pragma once

#include "sdk/Types.hpp"
#include "toml++/impl/node.hpp"

enum IconParseResult {
    InvalidToml,
    FailedMessage,
    FailedUnexpected,
    Success
};

class IconParser {
    std::string errorMessage;
    UTexture2D* texture;
    IconParseResult result;

    void parseInternal(std::string content, const std::string& modName);

public:
    explicit IconParser(toml::node_view<const toml::node> node, const std::string& modName);
    explicit IconParser(const std::string& content, const std::string& modName);

    [[nodiscard]] IconParseResult getResult() const;
    [[nodiscard]] std::string getErrorMessage() const;
    [[nodiscard]] UTexture2D* getTexture() const;
};
