
#pragma once

#include "Mod/CppUserModBase.hpp"

class SN2CustomCraft : public CppUserModBase {
    bool scanning = true;

    static void startup();

public:
    SN2CustomCraft();
    ~SN2CustomCraft() override;

    auto on_update() -> void override;
    auto on_lua_start(LuaMadeSimple::Lua &lua, LuaMadeSimple::Lua &main_lua, LuaMadeSimple::Lua &async_lua, LuaMadeSimple::Lua *hook_lua) -> void override;
};

#define MOD_EXPORT __declspec(dllexport)
extern "C" {
MOD_EXPORT inline CppUserModBase* start_mod(){ return new SN2CustomCraft(); }
MOD_EXPORT inline void uninstall_mod(const CppUserModBase* mod) { delete mod; }
}
