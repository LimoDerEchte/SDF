
---@meta

---@class SDFRecipe
SDFRecipe = {}

---@param Name string
function SDFRecipe:setName(Name) end

---@param Description string
function SDFRecipe:setDescription(Description) end

---@param Category string
function SDFRecipe:setCategory(Category) end

---@param Category UUWECraftingRecipeCategory
function SDFRecipe:setCategory(Category) end

---@param Icon string
function SDFRecipe:setIcon(Icon) end

---@param Icon UTexture2D
function SDFRecipe:setIcon(Icon) end

---@param Icon TSoftObjectPtr<UTexture2D>
function SDFRecipe:setIcon(Icon) end

---@param Item string
---@param Amount integer
function SDFRecipe:addIngredient(Item, Amount) end

---@param Item UUWEItemType
---@param Amount integer
function SDFRecipe:addIngredient(Item, Amount) end

---@param Item string
---@param Amount integer
function SDFRecipe:addOutput(Item, Amount) end

---@param Item UUWEItemType
---@param Amount integer
function SDFRecipe:addOutput(Item, Amount) end

---@param SetId string
---@param Item string
function SDFRecipe:addItemPickupUnlockingRequirement(SetId, Item) end

---@param SetId string
---@param Item UUWEItemType
function SDFRecipe:addItemPickupUnlockingRequirement(SetId, Item) end

---@param SetId string
---@param ScanData string
function SDFRecipe:addScanDataUnlockingRequirement(SetId, ScanData) end

---@param SetId string
---@param ScanData UUWEScanData
function SDFRecipe:addScanDataUnlockingRequirement(SetId, ScanData) end

function SDFRecipe:removeUnlockingRequirements() end

---@param Duration float
function SDFRecipe:setCraftingTime(Duration) end

---@param Available boolean
function SDFRecipe:setAvailableInLifepod(Available) end

---@param Index integer
function SDFRecipe:setOrderingIndex(Index) end

---@return UUWECraftingRecipe
function SDFRecipe:createOrModifyRecipe() end
