//
// Created by Limo on 25/05/2026.
//

#pragma once

#include "SDK/Subnautica2_classes.hpp"
#include "SDK/UWECrafting_classes.hpp"

class BuilderActionFactory {
    static std::vector<SDK::USN2BuilderActionData*> registeredActions;

    std::string id;
    std::string secondaryDescription;

    std::optional<std::string> powerDrainText, powerGenerationText;
    SDK::UUWECraftingRecipe* recipe;

    bool modifyMode;
    bool removePowerText = false;
    bool recipeModified = false;
    bool modifySecondaryDescription = false;

    friend class Hooks;

public:
    explicit BuilderActionFactory(std::string id, bool modifyMode);

    void setRemovePowerText(bool removePowerText);
    void setSecondaryDescription(const std::string& secondaryDescription);

    void addPowerDrainText(const std::string& text);
    void addPowerGenerationText(const std::string& text);

    bool setRecipe(const std::string& recipe);
    bool setRecipe(SDK::UUWECraftingRecipe* recipe);

    [[nodiscard]] SDK::USN2BuilderConstructActionData* registerBuilderAction() const;
};
