
#pragma once

#include "registering/Hooks.hpp"
#include "registering/RecipeFactory.hpp"

#include <DynamicOutput/Output.hpp>
#include <Mod/CppUserModBase.hpp>

#include "UnrealDef.hpp"
#include "parsing/CategoryParser.hpp"
#include "parsing/FileTraversal.hpp"
#include "parsing/RecipeParser.hpp"
#include "registering/BuilderActionFactory.hpp"
#include "registering/ItemTypeFactory.hpp"
#include "util/IconBuilder.hpp"
#include "util/Log.hpp"

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
