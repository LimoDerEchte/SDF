//
// Created by Limo on 25/05/2026.
//

#pragma once

#include "SDK/Subnautica2_classes.hpp"

class BuilderActionFactory {
    static std::vector<SDK::USN2BuilderActionData*> registeredActions;

    std::string recipeId;
    std::optional<std::string> powerDrainText, powerGenerationText;

    friend class Hooks;

public:
    explicit BuilderActionFactory(std::string recipeId);

    void addPowerDrainText(std::string text);
    void addPowerGenerationText(std::string text);

    [[nodiscard]] SDK::USN2BuilderConstructActionData* registerBuilderAction() const;
};
