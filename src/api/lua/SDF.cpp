//
// Created by Limo on 27/06/2026.
//

#include "SDF.hpp"

using namespace RC;

void SDF_Lua::RegisterLuaTypes(const LuaMadeSimple::Lua &lua, const LuaMadeSimple::Lua &main_lua, const LuaMadeSimple::Lua &async_lua, const LuaMadeSimple::Lua *hook_lua) {
    const auto L = main_lua.get_lua_state();
    lua_register(L, "HookEvent", Lua_HookEvent);
}

int SDF_Lua::Lua_HookEvent(lua_State *L) {
    luaL_checktype(L, 1, LUA_TFUNCTION);

    return 1;
}
