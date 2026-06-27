//
// Created by Limo on 27/06/2026.
//

#include "SDF.hpp"

#include "api/cpp/SDF.hpp"

using namespace RC;
using namespace Unreal;

LuaMadeSimple::Lua *SDF_Lua::hook_lua = nullptr;

void SDF_Lua::RegisterLuaTypes(const LuaMadeSimple::Lua &lua, const LuaMadeSimple::Lua &main_lua, const LuaMadeSimple::Lua &async_lua, LuaMadeSimple::Lua *hook_lua) {
    const auto L = hook_lua->get_lua_state();
    SDF_Lua::hook_lua = hook_lua;

    lua_register(L, "HookEvent", Lua_HookEvent);
}

int SDF_Lua::Lua_HookEvent(lua_State *L) {
    luaL_checktype(L, 1, LUA_TFUNCTION);

    lua_pushvalue(L, 1);
    int callbackRef = luaL_ref(L, LUA_REGISTRYINDEX);

    const auto handle = SDF_Impl::InternalInstance()->HookEvent([L, callbackRef](const SDF::Event event){
        lua_rawgeti(L, LUA_REGISTRYINDEX, callbackRef);
        lua_pushinteger(L, event);

        if (lua_pcall(L, 1, 0, 0) != LUA_OK)
        {
            printf("%s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    });

    lua_pushinteger(L, handle);
    return 1;
}
