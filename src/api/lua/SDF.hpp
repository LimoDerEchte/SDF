//
// Created by Limo on 27/06/2026.
//

#pragma once

#include "LuaMadeSimple/LuaMadeSimple.hpp"

class SDF_Lua {
public:
    static void RegisterLuaTypes(RC::LuaMadeSimple::Lua &lua, RC::LuaMadeSimple::Lua &main_lua, RC::LuaMadeSimple::Lua &async_lua, RC::LuaMadeSimple::Lua *hook_lua);
};
