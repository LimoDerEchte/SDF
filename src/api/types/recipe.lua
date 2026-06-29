
---@meta

---@class SDFRecipe
SDFRecipe = {}

---@param Name string
function SDFRecipe:setName(Name) end

---@param Description string
function SDFRecipe:setDescription(Description) end

---@param Category string
function SDFRecipe:setCategory(Category) end

---@param Category UObject
function SDFRecipe:setCategory(Category) end

---@return UUWECraftingRecipe
function SDFRecipe:createOrModifyRecipe() end
