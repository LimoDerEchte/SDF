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
    const auto [object, arg] = LuaStaticsSDF::parse_string_arg<SDFRecipe_Lua>(lua, "SDFRecipe:setName");
    object.ref->setName(arg);
    return 0;
}

int SDFRecipe_Lua::setDescription(const Lua &lua) {
    const auto [object, arg] = LuaStaticsSDF::parse_string_arg<SDFRecipe_Lua>(lua, "SDFRecipe:setDescription");
    object.ref->setDescription(arg);
    return 0;
}

int SDFRecipe_Lua::setCategory(const Lua &lua) {
    const auto [object, arg] = LuaStaticsSDF::parse_string_or_object_arg<SDFRecipe_Lua>(lua, "SDFRecipe:setCategory");

    if (const auto text = std::get_if<std::string>(&arg))
        object.ref->setCategory(*text);
    else if (const auto uObj = std::get_if<UObject*>(&arg))
        object.ref->setCategory(*uObj);
    return 0;
}

int SDFRecipe_Lua::createOrModifyRecipe(const Lua &lua) {
    const auto& lua_object = LuaStaticsSDF::parse_userdata<SDFRecipe_Lua>(lua, "SDFRecipe:createOrModifyRecipe");

    LuaType::auto_construct_object(lua, lua_object.ref->createOrModifyRecipe());
    return 1;
}

GenericObjectSource(SDFRecipe)

void SDFRecipe_Lua::setupFunctions(const Lua::Table &table) {
    GenericTypeFunction(SDFRecipe)
    table.add_pair("setName", setName);
    table.add_pair("setDescription", setDescription);
    table.add_pair("setCategory", setCategory);
    table.add_pair("createOrModifyRecipe", createOrModifyRecipe);
}
