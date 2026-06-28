//
// Created by Limo on 27/06/2026.
//

#include "SDF.hpp"

#include "api/cpp/SDF.hpp"

#include "LuaStatics.hpp"
#include "Mod/LuaMod.hpp"

using namespace RC;
using namespace Unreal;

LuaMadeSimple::Lua *SDF_Lua::hook_lua = nullptr;

void SDF_Lua::Lua_BuildEventTable(const LuaMadeSimple::Lua::Table &table) {
    table.add_pair("PreTraverse", 0);
    table.add_pair("PostTraverse", 1);
    table.add_pair("PreCategory", 2);
    table.add_pair("PostCategory", 3);
    table.add_pair("PreRecipe", 4);
    table.add_pair("PostRecipe", 5);
    table.add_pair("PreDatabankEntry", 6);
    table.add_pair("PostDatabankEntry", 7);
}

int SDF_Lua::Lua_HookEvent(const LuaMadeSimple::Lua &lua) {
    if (!lua.is_function())
        lua.throw_error("Parameter #1 for function 'SDF_HookEvent' must be a function");

    const auto mod = get_mod_ref(lua);
    auto [hook_lua, thread_ref] = LuaStaticsSDF::make_hook_state(mod);

    lua_pushvalue(lua.get_lua_state(), 1);
    lua_xmove(lua.get_lua_state(), hook_lua->get_lua_state(), 1);

    const auto funcRef = hook_lua->registry().make_ref();

    const auto hookId = SDF::HookEvent([hook_lua, funcRef](const SDF::Event event) {
        hook_lua->registry().get_function_ref(funcRef);
        hook_lua->set_integer(event);
        hook_lua->call_function(1, 0);
    });

    lua.set_integer(hookId);
    return 1;
}

int SDF_Lua::Lua_Unhook(const LuaMadeSimple::Lua &lua) {
    if (!lua.is_integer())
        lua.throw_error("Parameter #1 for function 'SDF_Unhook' must be an integer");

    const auto hookId = lua.get_integer();
    SDF::Unhook(hookId);
    return 0;
}

void SDF_Lua::RegisterLuaTypes(const LuaMadeSimple::Lua &lua) {
    const LuaType type(lua);

    type.add_table("Event", Lua_BuildEventTable);

    type.add_function("HookEvent", Lua_HookEvent);
    type.add_function("Unhook", Lua_Unhook);

    type.make_global("SDF");
}
