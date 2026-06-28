//
// Created by Limo on 28/06/2026.
//

#include "SDFRecipe.hpp"

#include "SDF/SDFRecipe.hpp"
#include "LuaMadeSimple/LuaObject.hpp"
#include "LuaType/LuaAActor.hpp"
#include "util/Log.hpp"

using namespace RC;
using namespace LuaMadeSimple;

int SDFRecipe_Lua::setName(const Lua &lua) {
    Log::Warning("Test");
    //const auto& lua_object = lua.get_userdata<SDFRecipe_Lua>();

    //Log::Warning("Test {:p}", (void*)lua_object.ref.get());

    //if (!lua.is_string())
    //    lua.throw_error("Parameter #1 for function 'SDFRecipe:setName' must be an integer");

    //lua_object.ref->setName(std::string(lua.get_string()));
    return 0;
}

int SDFRecipe_Lua::setDescription(const Lua &lua) {
    const auto& lua_object = lua.get_userdata<SDFRecipe_Lua>();

    if (!lua.is_string())
        lua.throw_error("Parameter #1 for function 'SDFRecipe:setDescription' must be an integer");

    lua_object.ref->setDescription(std::string(lua.get_string()));
    return 0;
}

int SDFRecipe_Lua::createOrModifyRecipe(const Lua &lua) {
    const auto& lua_object = lua.get_userdata<SDFRecipe_Lua>();

    LuaType::auto_construct_object(lua, lua_object.ref->createOrModifyRecipe());
    return 1;
}

SDFRecipe_Lua::SDFRecipe_Lua(std::unique_ptr<SDFRecipe>&& impl) : BaseObject("SDFRecipe"), ref(std::move(impl)) { }

auto SDFRecipe_Lua::construct(const Lua &lua, std::unique_ptr<SDFRecipe> &&impl) -> Lua::Table {
    SDFRecipe_Lua object {std::move(impl)};

    const Lua::Table table = lua.get_metatable("SDFRecipe");
    if (lua.is_nil(-1))
    {
        lua.discard_value(-1);
        lua.prepare_new_table();
        setupFunctions(table);
        lua.new_metatable<SDFRecipe>("SDFRecipe", object.get_metamethods());
    }

    lua.transfer_stack_object(std::move(object), "SDFRecipe", object.get_metamethods());
    return table;
}

void SDFRecipe_Lua::setupFunctions(const Lua::Table &table) {
    table.add_pair("setName", setName);
    table.add_pair("setDescription", setDescription);
    table.add_pair("createOrModifyRecipe", createOrModifyRecipe);

    table.add_pair("type", [](const Lua& lua) -> int {
        lua.set_string("SDFRecipe");
        return 1;
    });

    table.make_global("SDFRecipe");
}
