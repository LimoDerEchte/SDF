//
// Created by Limo on 27/06/2026.
//

#include "SDF.hpp"

#include "LuaType/LuaUObject.hpp"

#include "api/cpp/SDF.hpp"

#include "LuaStatics.hpp"
#include "SDFRecipe.hpp"
#include "Mod/LuaMod.hpp"

using namespace RC;
using namespace Unreal;

LuaMadeSimple::Lua *SDF_Lua::hook_lua = nullptr;

const auto EventTable = std::map<std::string, int>{
    { "PreTraverse", 0 },
    { "PostTraverse", 1 },
    { "PreCategory", 2 },
    { "PostCategory", 3 },
    { "PreRecipe", 4 },
    { "PostRecipe", 5 },
    { "PreDatabankEntry", 6 },
    { "PostDatabankEntry", 7 },
};

const auto AssetTypeTable = std::map<std::string, int>{
    { "Recipe", 0 },
    { "Category", 1 },
    { "DatabankEntry", 2 },
};

int SDF_Lua::Lua_HookEvent(const LuaMadeSimple::Lua &lua) {
    if (!lua.is_function())
        lua.throw_error("Parameter #1 for function 'SDF.HookEvent' must be a function");

    const auto mod = get_mod_ref(lua);
    auto [hook_lua, thread_ref] = LuaStaticsSDF::make_hook_state(mod);

    lua_pushvalue(lua.get_lua_state(), 1);
    lua_xmove(lua.get_lua_state(), hook_lua->get_lua_state(), 1);

    const auto funcRef = hook_lua->registry().make_ref();

    const auto hookId = SDF::HookEvent([hook_lua, funcRef](const SDF::Event event) {
        hook_lua->registry().get_function_ref(funcRef);
        // Args
        hook_lua->set_integer(event);
        // Invoke
        hook_lua->call_function(1, 0);
    });

    lua.set_integer(hookId);
    return 1;
}

int SDF_Lua::Lua_HookCreateAsset(const LuaMadeSimple::Lua &lua) {
    if (!lua.is_function())
        lua.throw_error("Parameter #1 for function 'SDF.HookCreateAsset' must be a function");

    const auto mod = get_mod_ref(lua);
    auto [hook_lua, thread_ref] = LuaStaticsSDF::make_hook_state(mod);

    lua_pushvalue(lua.get_lua_state(), 1);
    lua_xmove(lua.get_lua_state(), hook_lua->get_lua_state(), 1);

    const auto funcRef = hook_lua->registry().make_ref();

    const auto hookId = SDF::HookCreateAsset([hook_lua, funcRef](const SDF::AssetType type, const std::string &id, UObject* object) {
        hook_lua->registry().get_function_ref(funcRef);
        // Args
        hook_lua->set_integer(type);
        hook_lua->set_string(id);
        LuaType::auto_construct_object(*hook_lua, object);
        // Invoke
        hook_lua->call_function(3, 0);
    });

    lua.set_integer(hookId);
    return 1;
}

int SDF_Lua::Lua_Unhook(const LuaMadeSimple::Lua &lua) {
    if (!lua.is_integer())
        lua.throw_error("Parameter #1 for function 'SDF.Unhook' must be an integer");

    const auto hookId = lua.get_integer();
    SDF::Unhook(hookId);
    return 0;
}

int SDF_Lua::Lua_CreateRecipe(const LuaMadeSimple::Lua &lua) {
    if (!lua.is_string())
        lua.throw_error("Parameter #1 for function 'SDF.CreateRecipe' must be a string");

    SDFRecipe_Lua::construct(lua, SDF::CreateRecipe(std::string(lua.get_string())));
    return 1;
}

int SDF_Lua::Lua_ModifyRecipe(const LuaMadeSimple::Lua &lua) {
    if (!lua.is_string())
        lua.throw_error("Parameter #1 for function 'SDF.ModifyRecipe' must be a string");

    SDFRecipe_Lua::construct(lua, SDF::ModifyRecipe(std::string(lua.get_string())));
    return 1;
}

int SDF_Lua::Lua_DeleteRecipe(const LuaMadeSimple::Lua &lua) {
    if (!lua.is_string())
        lua.throw_error("Parameter #1 for function 'SDF.ModifyRecipe' must be a string");

    SDF::DeleteRecipe(std::string(lua.get_string()));
    return 0;
}

void SDF_Lua::RegisterLuaTypes(const LuaMadeSimple::Lua &lua) {
    const LuaTypeFactory type(lua);

    type.add_enum("Event", EventTable);
    type.add_enum("AssetType", AssetTypeTable);

    type.add_function("HookEvent", Lua_HookEvent);
    type.add_function("HookCreateAsset", Lua_HookCreateAsset);
    type.add_function("Unhook", Lua_Unhook);

    type.add_function("CreateRecipe", Lua_CreateRecipe);
    type.add_function("ModifyRecipe", Lua_ModifyRecipe);
    type.add_function("DeleteRecipe", Lua_DeleteRecipe);

    type.make_global("SDF");
}
