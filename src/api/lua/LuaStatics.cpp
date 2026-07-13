//
// Created by Limo on 28/06/2026.
//

#include "LuaStatics.hpp"

#include "LuaType/LuaFSoftObjectPath.hpp"
#include "LuaType/LuaTSoftObjectPtr.hpp"
#include "SDK/CoreUObject_classes.hpp"
#include "util/Log.hpp"

int LuaStaticsSDF::ensure_hook_thread_exists(LuaMod *mod) {
    if (mod->m_hook_lua == nullptr)
    {
        mod->m_hook_lua = &mod->lua().new_thread();
        const int thread_ref = luaL_ref(mod->lua().get_lua_state(), LUA_REGISTRYINDEX);
        return thread_ref;
    }
    return LUA_REFNIL;
}

std::pair<LuaMadeSimple::Lua*, int> LuaStaticsSDF::make_hook_state(LuaMod *mod) {
    int thread_ref = ensure_hook_thread_exists(mod);
    return {mod->m_hook_lua, thread_ref};
}

std::string LuaStaticsSDF::parse_string_arg(const LuaMadeSimple::Lua &lua, const std::string &funcName, int index) {
    if (!lua.is_string())
        lua.throw_error(std::format("Parameter #{} for function '{}' must be a string", index, funcName));
    return std::string(lua.get_string());
}

int64_t LuaStaticsSDF::parse_int_arg(const LuaMadeSimple::Lua &lua, const std::string &funcName, int index) {
    if (!lua.is_integer())
        lua.throw_error(std::format("Parameter #{} for function '{}' must be an integer", index, funcName));
    return lua.get_integer();
}

double LuaStaticsSDF::parse_double_arg(const LuaMadeSimple::Lua &lua, const std::string &funcName, int index) {
    if (!lua.is_number())
        lua.throw_error(std::format("Parameter #{} for function '{}' must be a floating point number", index, funcName));
    return lua.get_number();
}

bool LuaStaticsSDF::parse_bool_arg(const LuaMadeSimple::Lua &lua, const std::string &funcName, int index) {
    if (!lua.is_bool())
        lua.throw_error(std::format("Parameter #{} for function '{}' must be a boolean", index, funcName));
    return lua.get_bool();
}

std::variant<std::string, Unreal::UObject*> LuaStaticsSDF::parse_string_or_object_arg(const LuaMadeSimple::Lua &lua, const std::string &funcName, int index) {
    if (lua.is_string())
        return std::string(lua.get_string());

    if (!lua.is_userdata())
        lua.throw_error(std::format("Parameter #{} for function '{}' must be a string, UObject or reference", index, funcName));

    const auto& userdata = lua.get_userdata<LuaType::UE4SSBaseObject>(1, true);
    const auto name = std::string_view(userdata.get_object_name());

    if (name == "UObject")
        return reinterpret_cast<Unreal::UObject*>(userdata.get_remote_cpp_object());

    if (name == "TSoftObjectPtr")
        return reinterpret_cast<Unreal::TSoftObjectPtr<>*>(userdata.get_remote_cpp_object())->LoadSynchronous();

    lua.throw_error(std::format("Parameter #{} for function '{}' must be a string, UObject or reference", index, funcName));
    throw std::runtime_error("Unreachable");
}

std::variant<std::string, Unreal::UObject*> LuaStaticsSDF::parse_string_or_object_or_ref_arg(const LuaMadeSimple::Lua &lua, const std::string &funcName, int index) {
    if (lua.is_string())
        return std::string(lua.get_string());

    if (!lua.is_userdata())
        lua.throw_error(std::format("Parameter #{} for function '{}' must be a string, UObject or SoftObjectPtr", index, funcName));
    const auto& userdata = lua.get_userdata<LuaType::UE4SSBaseObject>(1, true);

    if (std::string_view(userdata.get_object_name()) == "UObject")
        return lua.get_userdata<LuaType::UObject>().get_remote_cpp_object();


    Log::Warning("{}", userdata.get_object_name());

    //throw std::runtime_error(std::format("Parameter #{} for function '{}' must be a string, UObject or SoftObjectPtr", index, funcName));
    return nullptr;
}

LuaTypeFactory::LuaTypeFactory(const LuaMadeSimple::Lua &lua) : lua(lua) {
    lua_createtable(lua.get_lua_state(), 0, 0);
}

void LuaTypeFactory::add_enum(const std::string &key, const std::map<std::string, int> &entries) const {
    const auto ta = lua.prepare_new_table();

    for (const auto&[key, value] : entries)
        ta.add_pair(key.c_str(), value);

    lua_setfield(lua.get_lua_state(), -2, key.c_str());
}

void LuaTypeFactory::add_function(const std::string &key, const LuaMadeSimple::Lua::LuaFunction& function) const {
    lua.register_function(key, function);

    lua_getglobal(lua.get_lua_state(), key.c_str());
    lua_setfield(lua.get_lua_state(), -2, key.c_str());

    lua.set_nil();
    lua_setglobal(lua.get_lua_state(), key.c_str());
}

void LuaTypeFactory::make_global(const std::string &key) const {
    lua_setglobal(lua.get_lua_state(), key.c_str());
}
