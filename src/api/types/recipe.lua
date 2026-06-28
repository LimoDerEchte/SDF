
---@meta

---@class SDFRecipe
SDFRecipe = {}

---@param Name string
function SDFRecipe:setName(Name) end

---@param Description string
function SDFRecipe:setDescription(Description) end

---@return UUWECraftingRecipe
function SDFRecipe:createOrModifyRecipe() end
