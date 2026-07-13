//
// Created by Limo on 25/05/2026.
//

#pragma once

#include "sdk/Types.hpp"

class BuilderActionFactory {
    static std::vector<USN2BuilderActionData*> registeredActions;

    std::string id;
    std::optional<std::string> secondaryDescription;

    std::optional<std::string> powerDrainText, powerGenerationText;
    UUWECraftingRecipe* recipe;

    bool modifyMode;
    bool removePowerText = false;
    bool recipeModified = false;

    friend class Hooks;

public:
    explicit BuilderActionFactory(std::string id, bool modifyMode);

    void setRemovePowerText(bool removePowerText);
    void setSecondaryDescription(const std::string& secondaryDescription);

    void addPowerDrainText(const std::string& text);
    void addPowerGenerationText(const std::string& text);

    bool setRecipe(const std::string& recipe);
    bool setRecipe(UUWECraftingRecipe* recipe);

    [[nodiscard]] USN2BuilderConstructActionData* registerBuilderAction() const;
};
