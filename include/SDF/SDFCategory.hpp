//
// Created by Limo on 28/06/2026.
//

#pragma once

#include <string>

#include "SoftObjectPtr.hpp"

class SDFCategory {
public:
    virtual ~SDFCategory() = default;

    /**
     * Sets the displayed name of the crafting recipe category
     * @param name Name to display
     **/
    virtual void setName(const std::string &name) = 0;

    /**
     * Sets the displayed description of the crafting recipe category
     * @param description Description to display
     **/
    [[deprecated("The category description is currently unused in the game and can mostly be ignored")]]
    virtual void setDescription(const std::string &description) = 0;

    /**
     * Sets the categories crafter type, which is required for root categories and processor categories
     * @param type An integer corresponding to the CrafterType enum
     **/
    virtual void setCrafterType(uint8_t type) = 0;

    /**
     * Assigns a parent category to the crafting recipe category
     * @param category ID of the category according to the category notation: https://sn2-sdf.dev/generic/notations/#crafting-recipe-categories
     */
    virtual void setParent(const std::string &category) = 0;

    /**
     * Assigns a parent category to the crafting recipe category
     * @param category An object of type UWECraftingRecipeCategory to use as the parent category
     */
    virtual void setParent(RC::Unreal::UObject *category) = 0;

    /**
     * Assigns an icon to the crafting recipe category
     * @param icon An icon according to the icon notation: https://sn2-sdf.dev/generic/icons/
     */
    virtual void setIcon(const std::string &icon) = 0;

    /**
     * Assigns an icon to the crafting recipe category
     * @param icon An object of type Texture2D to use as the categories icon
     */
    virtual void setIcon(RC::Unreal::UObject *icon) = 0;

    /**
     * Assigns an icon to the crafting recipe category
     * @param icon A soft object reference to an object of type Texture2D to use as the categories icon
     */
    virtual void setIcon(const RC::Unreal::TSoftObjectPtr<> &icon) = 0;

    /**
     * Sets the categories ordering index, that decides the order the categories are shown in - The higher the ordering index, the lower the category will be
     * @param orderingIndex Ordering index to assign
     */
    virtual void setOrderingIndex(int orderingIndex) = 0;

    /**
     * Executes everything that was executed on this builder
     * @return Returns either the registered / modified object or nullptr if the creation failed due to being executed too early
     */
    [[nodiscard]] virtual RC::Unreal::UObject* createOrModifyCategory() = 0;
};
