
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

---@param Callback fun(event: SDF.Event)
---@return int64
function SDF.HookEvent(Callback) end

---@param HookId int64
function SDF.Unhook(HookId) end
