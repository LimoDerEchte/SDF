//
// Created by Limo on 27/06/2026.
//

#pragma once

#include "LuaMadeSimple/LuaMadeSimple.hpp"

class SDF_Lua {
public:
    static void RegisterLuaTypes(const RC::LuaMadeSimple::Lua &lua, const RC::LuaMadeSimple::Lua &main_lua, const RC::LuaMadeSimple::Lua &async_lua, const RC::LuaMadeSimple::Lua *hook_lua);

    static int Lua_HookEvent(lua_State *L);
};
