
---@meta

--- This class is used to build certain aspects of a crafting recipe before actually creating it, so it is much less manual labor
---@class SDFRecipe
SDFRecipe = {}

--- Sets the displayed name of the crafting recipe
---@param Name string Name to display
function SDFRecipe:setName(Name) end

--- Sets the displayed description of the crafting recipe
---@param Description string Description to display
function SDFRecipe:setDescription(Description) end

--- Assigns a category to the crafting recipe
---@param Category string ID of the category according to the category notation: https://sn2-sdf.dev/generic/notations/#crafting-recipe-categories
function SDFRecipe:setCategory(Category) end

--- Assigns a category to the crafting recipe
---@param Category UUWECraftingRecipeCategory The category to use
function SDFRecipe:setCategory(Category) end

--- Assigns an icon to the crafting recipe
---@param Icon string An icon according to the icon notation: https://sn2-sdf.dev/generic/icons/
function SDFRecipe:setIcon(Icon) end

--- Assigns an icon to the crafting recipe
---@param Icon UTexture2D The texture to use
function SDFRecipe:setIcon(Icon) end

--- Assigns an icon to the crafting recipe
---@param Icon TSoftObjectPtr<UTexture2D> Soft reference to the texture to use
function SDFRecipe:setIcon(Icon) end

--- Adds an ingredient to the crafting recipe
---@param Item string ID of the item according to the item notation: https://sn2-sdf.dev/generic/notations/#item-types
---@param Amount integer The amount of the specified item
function SDFRecipe:addIngredient(Item, Amount) end

--- Adds an ingredient to the crafting recipe
---@param Item UUWEItemType Item type to add
---@param Amount integer The amount of the specified item
function SDFRecipe:addIngredient(Item, Amount) end

--- Adds an output to the crafting recipe
--- [Warning] Using more than one output type per item is not recommended
---@param Item string ID of the item according to the item notation: https://sn2-sdf.dev/generic/notations/#item-types
---@param Amount integer The amount of the specified item
function SDFRecipe:addOutput(Item, Amount) end

--- Adds an output to the crafting recipe
--- [Warning] Using more than one output type per item is not recommended
---@param Item UUWEItemType Item type to add
---@param Amount integer The amount of the specified item
function SDFRecipe:addOutput(Item, Amount) end

--- Adds an item pickup rule to a set of unlocking requirements of this crafting recipe
---@param SetId string Set id of the requirement without any specific meaning. Rules with the same set id are combined to one set.
---@param Item string ID of the item according to the item notation: https://sn2-sdf.dev/generic/notations/#item-types
function SDFRecipe:addItemPickupUnlockingRequirement(SetId, Item) end

--- Adds an item pickup rule to a set of unlocking requirements of this crafting recipe
---@param SetId string Set id of the requirement without any specific meaning. Rules with the same set id are combined to one set.
---@param Item UUWEItemType Item type to use for the rule
function SDFRecipe:addItemPickupUnlockingRequirement(SetId, Item) end

--- Adds a scan data rule to a set of unlocking requirements of this crafting recipe
---@param SetId string Set id of the requirement without any specific meaning. Rules with the same set id are combined to one set.
---@param ScanData string ID of the scan data according to the scan data notation: https://sn2-sdf.dev/generic/notations/#scan-data
function SDFRecipe:addScanDataUnlockingRequirement(SetId, ScanData) end

--- Adds a scan data rule to a set of unlocking requirements of this crafting recipe
---@param SetId string Set id of the requirement without any specific meaning. Rules with the same set id are combined to one set.
---@param ScanData UUWEScanData Scan data to use for the rule
function SDFRecipe:addScanDataUnlockingRequirement(SetId, ScanData) end

--- Removes all unlocking requirements - Only relevant in modify mode if no rules are changed
function SDFRecipe:removeUnlockingRequirements() end

--- Sets the duration it takes for this item to be crafted
---@param Duration float Duration in seconds
function SDFRecipe:setCraftingTime(Duration) end

--- Sets if the recipe should be available for crafting inside the lifepod fabricator
---@param Available boolean The availability of this recipe in the lifepod
function SDFRecipe:setAvailableInLifepod(Available) end

--- Sets the recipes ordering index, that decides the order the recipes are shown in. The higher the ordering index, the lower the recipe will be.
---@param Index integer Ordering index to assign
function SDFRecipe:setOrderingIndex(Index) end

--- Executes everything that was executed on this builder
---@return UUWECraftingRecipe Returns Either the registered / modified object or nil if the creation failed due to being executed too early
function SDFRecipe:createOrModifyRecipe() end
