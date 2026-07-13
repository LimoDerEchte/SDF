//
// Created by Limo on 28/06/2026.
//

#include "SDFRecipe.hpp"

#include "SDF/SDFRecipe.hpp"

#include "UnrealDef.hpp"
#include "LuaMadeSimple/LuaObject.hpp"
#include "LuaType/LuaAActor.hpp"

using namespace RC;
using namespace LuaMadeSimple;

int SDFRecipe_Lua::setName(const Lua &lua) {
    const auto& self = LuaStaticsSDF::parse_self<SDFRecipe_Lua>(lua, "SDFRecipe:setName");
    const auto arg = LuaStaticsSDF::parse_string_arg(lua, "SDFRecipe:setName", 1);
    self.ref->setName(arg);
    return 0;
}

int SDFRecipe_Lua::setDescription(const Lua &lua) {
    const auto& self = LuaStaticsSDF::parse_self<SDFRecipe_Lua>(lua, "SDFRecipe:setDescription");
    const auto arg = LuaStaticsSDF::parse_string_arg(lua, "SDFRecipe:setDescription", 1);
    self.ref->setDescription(arg);
    return 0;
}

int SDFRecipe_Lua::setCategory(const Lua &lua) {
    const auto& self = LuaStaticsSDF::parse_self<SDFRecipe_Lua>(lua, "SDFRecipe:setCategory");
    const auto arg = LuaStaticsSDF::parse_string_or_object_arg(lua, "SDFRecipe:setCategory", 1);

    if (const auto text = std::get_if<std::string>(&arg))
        self.ref->setCategory(*text);
    else if (const auto uObj = std::get_if<UObject*>(&arg))
        self.ref->setCategory(*uObj);
    return 0;
}

int SDFRecipe_Lua::setIcon(const Lua &lua) {
    const auto& self = LuaStaticsSDF::parse_self<SDFRecipe_Lua>(lua, "SDFRecipe:setIcon");
    const auto arg = LuaStaticsSDF::parse_string_or_object_or_ref_arg(lua, "SDFRecipe:setIcon", 1);

    if (const auto text = std::get_if<std::string>(&arg))
        self.ref->setIcon(*text);
    else if (const auto uObj = std::get_if<UObject*>(&arg))
        self.ref->setIcon(*uObj);
    else if (const auto uRef = std::get_if<Unreal::TSoftObjectPtr<>>(&arg))
        self.ref->setIcon(*uRef);
    return 0;
}

int SDFRecipe_Lua::addIngredient(const Lua &lua) {
    const auto& self = LuaStaticsSDF::parse_self<SDFRecipe_Lua>(lua, "SDFRecipe:addIngredient");
    const auto ingredient = LuaStaticsSDF::parse_string_or_object_arg(lua, "SDFRecipe:addIngredient", 1);
    const auto amount = static_cast<int>(LuaStaticsSDF::parse_int_arg(lua, "SDFRecipe:addIngredient", 2));

    if (const auto text = std::get_if<std::string>(&ingredient))
        self.ref->addIngredient(*text, amount);
    else if (const auto uObj = std::get_if<UObject*>(&ingredient))
        self.ref->addIngredient(*uObj, amount);
    return 0;
}

int SDFRecipe_Lua::addOutput(const Lua &lua) {
    const auto& self = LuaStaticsSDF::parse_self<SDFRecipe_Lua>(lua, "SDFRecipe:addOutput");
    const auto output = LuaStaticsSDF::parse_string_or_object_arg(lua, "SDFRecipe:addOutput", 1);
    const auto amount = static_cast<int>(LuaStaticsSDF::parse_int_arg(lua, "SDFRecipe:addOutput", 2));

    if (const auto text = std::get_if<std::string>(&output))
        self.ref->addOutput(*text, amount);
    else if (const auto uObj = std::get_if<UObject*>(&output))
        self.ref->addOutput(*uObj, amount);
    return 0;
}

int SDFRecipe_Lua::addItemPickupUnlockingRequirement(const Lua &lua) {
    const auto& self = LuaStaticsSDF::parse_self<SDFRecipe_Lua>(lua, "SDFRecipe:addItemPickupUnlockingRequirement");
    const auto setId = LuaStaticsSDF::parse_string_arg(lua, "SDFRecipe:addItemPickupUnlockingRequirement", 1);
    const auto arg = LuaStaticsSDF::parse_string_or_object_arg(lua, "SDFRecipe:addItemPickupUnlockingRequirement", 2);

    if (const auto text = std::get_if<std::string>(&arg))
        self.ref->addItemPickupUnlockingRequirement(setId, *text);
    else if (const auto uObj = std::get_if<UObject*>(&arg))
        self.ref->addItemPickupUnlockingRequirement(setId, *uObj);
    return 0;
}

int SDFRecipe_Lua::addScanDataUnlockingRequirement(const Lua &lua) {
    const auto& self = LuaStaticsSDF::parse_self<SDFRecipe_Lua>(lua, "SDFRecipe:addScanDataUnlockingRequirement");
    const auto setId = LuaStaticsSDF::parse_string_arg(lua, "SDFRecipe:addScanDataUnlockingRequirement", 1);
    const auto arg = LuaStaticsSDF::parse_string_or_object_arg(lua, "SDFRecipe:addScanDataUnlockingRequirement", 2);

    if (const auto text = std::get_if<std::string>(&arg))
        self.ref->addScanDataUnlockingRequirement(setId, *text);
    else if (const auto uObj = std::get_if<UObject*>(&arg))
        self.ref->addScanDataUnlockingRequirement(setId, *uObj);
    return 0;
}

int SDFRecipe_Lua::removeUnlockingRequirements(const Lua &lua) {
    const auto& self = LuaStaticsSDF::parse_self<SDFRecipe_Lua>(lua, "SDFRecipe:removeUnlockingRequirements");
    self.ref->removeUnlockingRequirements();
    return 0;
}

int SDFRecipe_Lua::setCraftingTime(const Lua &lua) {
    const auto& self = LuaStaticsSDF::parse_self<SDFRecipe_Lua>(lua, "SDFRecipe:setCraftingTime");
    const auto time = LuaStaticsSDF::parse_double_arg(lua, "SDFRecipe:setCraftingTime", 1);
    self.ref->setCraftingTime(static_cast<float>(time));
    return 0;
}

int SDFRecipe_Lua::setAvailableInLifepod(const Lua &lua) {
    const auto& self = LuaStaticsSDF::parse_self<SDFRecipe_Lua>(lua, "SDFRecipe:setAvailableInLifepod");
    const auto available = LuaStaticsSDF::parse_bool_arg(lua, "SDFRecipe:setAvailableInLifepod", 1);
    self.ref->setAvailableInLifepod(available);
    return 0;
}

int SDFRecipe_Lua::setOrderingIndex(const Lua &lua) {
    const auto& self = LuaStaticsSDF::parse_self<SDFRecipe_Lua>(lua, "SDFRecipe:setOrderingIndex");
    const auto index = LuaStaticsSDF::parse_int_arg(lua, "SDFRecipe:setOrderingIndex", 1);
    self.ref->setOrderingIndex(static_cast<int>(index));
    return 0;
}

int SDFRecipe_Lua::createOrModifyRecipe(const Lua &lua) {
    const auto& self = LuaStaticsSDF::parse_self<SDFRecipe_Lua>(lua, "SDFRecipe:createOrModifyRecipe");
    if (const auto ptr = self.ref->createOrModifyRecipe())
        LuaType::auto_construct_object(lua, ptr);
    else
        lua.set_nil();
    return 1;
}

GenericObjectSource(SDFRecipe)

void SDFRecipe_Lua::setupFunctions(const Lua::Table &table) {
    GenericTypeFunction(SDFRecipe)

    table.add_pair("setName", setName);
    table.add_pair("setDescription", setDescription);
    table.add_pair("setCategory", setCategory);
    table.add_pair("setIcon", setIcon);

    table.add_pair("addIngredient", addIngredient);
    table.add_pair("addOutput", addOutput);

    table.add_pair("addItemPickupUnlockingRequirement", addItemPickupUnlockingRequirement);
    table.add_pair("addScanDataUnlockingRequirement", addScanDataUnlockingRequirement);
    table.add_pair("removeUnlockingRequirements", removeUnlockingRequirements);

    table.add_pair("setCraftingTime", setCraftingTime);
    table.add_pair("setAvailableInLifepod", setAvailableInLifepod);
    table.add_pair("setOrderingIndex", setOrderingIndex);

    table.add_pair("createOrModifyRecipe", createOrModifyRecipe);
}
