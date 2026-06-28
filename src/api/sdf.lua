
---@meta

---@class SDF
SDF = {}

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

---@class SDF.AssetType
SDF.AssetType = {
	Recipe = 0,
	Category = 1,
	DatabankEntry = 2,
}

---@param Callback fun(event: SDF.Event)
---@return int64
function SDF.HookEvent(Callback) end

---@param Callback fun(type: SDF.AssetType, id: string, object: UObject)
---@return int64
function SDF.HookCreateAsset(Callback) end

---@param HookId int64
function SDF.Unhook(HookId) end
