//
// Created by Limo on 27/06/2026.
//

#pragma once

#include "LuaMadeSimple/LuaMadeSimple.hpp"

class SDF_Lua {
    static RC::LuaMadeSimple::Lua *hook_lua;

    static void Lua_BuildEventTable(const RC::LuaMadeSimple::Lua::Table &table);
    static void Lua_BuildAssetTypeTable(const RC::LuaMadeSimple::Lua::Table &table);

    static int Lua_HookEvent(const RC::LuaMadeSimple::Lua &lua);
    static int Lua_HookCreateAsset(const RC::LuaMadeSimple::Lua &lua);
    static int Lua_Unhook(const RC::LuaMadeSimple::Lua &lua);

public:
    static void RegisterLuaTypes(const RC::LuaMadeSimple::Lua &lua);
};
