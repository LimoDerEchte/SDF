
---@meta

--- This class is used to build certain aspects of a crafting recipe before actually creating it, so it is much less manual labor
---@class SDFRecipe
SDFRecipe = {}

--- Sets the displayed name of the crafting recipe
---@param Name string
function SDFRecipe:setName(Name) end

--- Sets the displayed description of the crafting recipe
---@param Description string
function SDFRecipe:setDescription(Description) end

--- Assigns a category to the crafting recipe
--- - The Category field follows the category notation: https://sn2-sdf.dev/generic/notations/#crafting-recipe-categories
---@param Category string
function SDFRecipe:setCategory(Category) end

--- Assigns a category to the crafting recipe
---@param Category UUWECraftingRecipeCategory
function SDFRecipe:setCategory(Category) end

--- Assigns an icon to the crafting recipe
--- - The Icon field follows the icon notation: https://sn2-sdf.dev/generic/icons/
---@param Icon string
function SDFRecipe:setIcon(Icon) end

--- Assigns an icon to the crafting recipe
---@param Icon UTexture2D
function SDFRecipe:setIcon(Icon) end

--- Assigns an icon to the crafting recipe
---@param Icon TSoftObjectPtr<UTexture2D>
function SDFRecipe:setIcon(Icon) end

--- Adds an ingredient to the crafting recipe
--- - The Item field follows the item notation: https://sn2-sdf.dev/generic/notations/#item-types
---@param Item string
---@param Amount integer
function SDFRecipe:addIngredient(Item, Amount) end

--- Adds an ingredient to the crafting recipe
---@param Item UUWEItemType
---@param Amount integer
function SDFRecipe:addIngredient(Item, Amount) end

--- Adds an output to the crafting recipe
--- - The Item field follows the item notation: https://sn2-sdf.dev/generic/notations/#item-types
--- - Warning: Using more than one output type per item is not recommended
---@param Item string
---@param Amount integer
function SDFRecipe:addOutput(Item, Amount) end

--- Adds an output to the crafting recipe
--- - Warning: Using more than one output type per item is not recommended
---@param Item UUWEItemType
---@param Amount integer
function SDFRecipe:addOutput(Item, Amount) end

--- Adds an item pickup rule to a set of unlocking requirements of this crafting recipe
--- - The Item field follows the item notation: https://sn2-sdf.dev/generic/notations/#item-types
---@param SetId string
---@param Item string
function SDFRecipe:addItemPickupUnlockingRequirement(SetId, Item) end

--- Adds an item pickup rule to a set of unlocking requirements of this crafting recipe
---@param SetId string
---@param Item UUWEItemType
function SDFRecipe:addItemPickupUnlockingRequirement(SetId, Item) end

--- Adds a scan data rule to a set of unlocking requirements of this crafting recipe
--- - The ScanData field follows the scan data notation: https://sn2-sdf.dev/generic/notations/#scan-data
---@param SetId string
---@param ScanData string
function SDFRecipe:addScanDataUnlockingRequirement(SetId, ScanData) end

--- Adds a scan data rule to a set of unlocking requirements of this crafting recipe
---@param SetId string
---@param ScanData UUWEScanData
function SDFRecipe:addScanDataUnlockingRequirement(SetId, ScanData) end

--- Removes all unlocking requirements - Only relevant in modify mode if no rules are changed
function SDFRecipe:removeUnlockingRequirements() end

--- Sets the duration it takes for this item to be crafted
---@param Duration float
function SDFRecipe:setCraftingTime(Duration) end

--- Sets if the recipe should be available for crafting inside the lifepod fabricator
---@param Available boolean
function SDFRecipe:setAvailableInLifepod(Available) end

--- Sets the recipes ordering index, that decides the order the recipes are shown in
--- - The higher the ordering index, the lower the recipe will be
---@param Index integer
function SDFRecipe:setOrderingIndex(Index) end

--- Executes everything that was executed on this builder
---@return UUWECraftingRecipe
function SDFRecipe:createOrModifyRecipe() end
