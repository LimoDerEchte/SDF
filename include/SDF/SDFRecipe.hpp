//
// Created by Limo on 28/06/2026.
//

#pragma once

#include <string>

#include "SoftObjectPtr.hpp"

/**
 * This class is used to build certain aspects of a crafting recipe before actually creating it, so it is much less manual labor
 */
class SDFRecipe {
public:
    virtual ~SDFRecipe() = default;

    /**
     * Sets the displayed name of the crafting recipe
     * @param name Name to display
     **/
    virtual void setName(const std::string &name) = 0;

    /**
     * Sets the displayed description of the crafting recipe
     * @param description Description to display
     **/
    virtual void setDescription(const std::string &description) = 0;

    /**
     * Assigns a category to the crafting recipe
     * @param category ID of the category according to the category notation: https://sn2-sdf.dev/generic/notations/#crafting-recipe-categories
     */
    virtual void setCategory(const std::string &category) = 0;

    /**
     * Assigns a category to the crafting recipe
     * @param category An object of type UWECraftingRecipeCategory to use as the recipes category
     */
    virtual void setCategory(RC::Unreal::UObject *category) = 0;

    /**
     * Assigns an icon to the crafting recipe
     * @param icon An icon according to the icon notation: https://sn2-sdf.dev/generic/icons/
     */
    virtual void setIcon(const std::string &icon) = 0;

    /**
     * Assigns an icon to the crafting recipe
     * @param icon An object of type Texture2D to use as the recipes icon
     */
    virtual void setIcon(RC::Unreal::UObject *icon) = 0;

    /**
     * Assigns an icon to the crafting recipe
     * @param icon A soft object reference to an object of type Texture2D to use as the recipes icon
     */
    virtual void setIcon(const RC::Unreal::TSoftObjectPtr<> &icon) = 0;

    /**
     * Adds an ingredient to the crafting recipe
     * @param ingredient ID of the item according to the item notation: https://sn2-sdf.dev/generic/notations/#item-types
     * @param amount The amount of the specified item
     */
    virtual void addIngredient(const std::string &ingredient, int32_t amount) = 0;

    /**
     * Adds an ingredient to the crafting recipe
     * @param ingredient An object of type UWEItemType to add to the recipes ingredients
     * @param amount The amount of the specified item
     */
    virtual void addIngredient(RC::Unreal::UObject *ingredient, int32_t amount) = 0;

    /**
     * Adds an output to the crafting recipe
     * @param output ID of the item according to the item notation: https://sn2-sdf.dev/generic/notations/#item-types
     * @param amount The amount of the specified item
     */
    virtual void addOutput(const std::string &output, int32_t amount) = 0;

    /**
     * Adds an output to the crafting recipe
     * @param output An object of type UWEItemType to add to the recipes outputs
     * @param amount The amount of the specified item
     */
    virtual void addOutput(RC::Unreal::UObject *output, int32_t amount) = 0;

    /**
     * Adds an item pickup rule to a set of unlocking requirements of this crafting recipe
     * @param setId Set id of the requirement without any specific meaning. Rules with the same set id are combined to one set.
     * @param item ID of the item according to the item notation: https://sn2-sdf.dev/generic/notations/#item-types
     */
    virtual void addItemPickupUnlockingRequirement(const std::string &setId, const std::string &item) = 0;

    /**
     * Adds an item pickup rule to a set of unlocking requirements of this crafting recipe
     * @param setId Set id of the requirement without any specific meaning. Rules with the same set id are combined to one set.
     * @param item An object of type UWEItemType to add to this set of unlocking rules
     */
    virtual void addItemPickupUnlockingRequirement(const std::string &setId, RC::Unreal::UObject *item) = 0;

    /**
     * Adds a scan data rule to a set of unlocking requirements of this crafting recipe
     * @param setId Set id of the requirement without any specific meaning. Rules with the same set id are combined to one set.
     * @param scanData ID of the scan data according to the scan data notation: https://sn2-sdf.dev/generic/notations/#scan-data
     */
    virtual void addScanDataUnlockingRequirement(const std::string &setId, const std::string &scanData) = 0;

    /**
     * Adds a scan data rule to a set of unlocking requirements of this crafting recipe
     * @param setId Set id of the requirement without any specific meaning. Rules with the same set id are combined to one set.
     * @param scanData An object of type UWEScanData to add to this set of unlocking rules
     */
    virtual void addScanDataUnlockingRequirement(const std::string &setId, RC::Unreal::UObject *scanData) = 0;

    /**
     * Removes all unlocking requirements - Only relevant in modify mode if no rules are changed
     */
    virtual void removeUnlockingRequirements() = 0;

    /**
     * Sets the duration it takes for this item to be crafted
     * @param time Duration in seconds
     */
    virtual void setCraftingTime(float time) = 0;

    /**
     * Sets if the recipe should be available for crafting inside the lifepod fabricator
     * @param available The availability of this recipe in the lifepod
     */
    virtual void setAvailableInLifepod(bool available) = 0;

    /**
     * Sets the recipes ordering index, that decides the order the recipes are shown in - The higher the ordering index, the lower the recipe will be
     * @param orderingIndex Ordering index to assign
     */
    virtual void setOrderingIndex(int orderingIndex) = 0;

    /**
     * Executes everything that was executed on this builder
     * @return Returns either the registered / modified object or nullptr if the creation failed due to being executed too early
     */
    [[nodiscard]] virtual RC::Unreal::UObject* createOrModifyRecipe() = 0;
};
