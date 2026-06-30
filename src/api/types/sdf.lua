
---@meta

--- This class represents the heart of the SDF Lua API and provides multiple static functions to use in your code
---@class SDF
SDF = {}

--- Represents a specific type of asset registered by SDF
---@class SDF.Event
SDF.Event = {
	PreTraverse = 0,
	PostTraverse = 1,
	PreCategory = 2,
	PostCategory = 3,
	PreRecipe = 4,
	PostRecipe = 5,
	PreDatabankEntry = 6,
	PostDatabankEntry = 7,
}

--- Represents a specific kind of event triggered at certain points in the SDF code
---@class SDF.AssetType
SDF.AssetType = {
	Recipe = 0,
	Category = 1,
	DatabankEntry = 2,
}

--- Registers a callback to be invoked every time an SDF event is fired
---@param Callback fun(event: SDF.Event)
---@return int64
function SDF.HookEvent(Callback) end

--- Registers a callback to be invoked every time SDF has registered or modified an asset
---@param Callback fun(type: SDF.AssetType, id: string, object: UObject)
---@return int64
function SDF.HookCreateAsset(Callback) end

--- Unregisters a previously registered hook and disposes of the callback
---@param HookId int64
function SDF.Unhook(HookId) end

--- Creates a builder that can be used for creating a new crafting recipe
---@param RecipeId string
---@return SDFRecipe
function SDF.CreateRecipe(RecipeId) end

--- Creates a builder that can be used for modifying an existing crafting recipe
---@param RecipeId string
---@return SDFRecipe
function SDF.ModifyRecipe(RecipeId) end

--- Moves an existing crafting recipe to an invalid category, effectively deleting it in the process
---@param RecipeId string
function SDF.DeleteRecipe(RecipeId) end
