//
// Created by Limo on 28/06/2026.
//

#include "LuaStatics.hpp"

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

template<typename T>
const T & LuaStaticsSDF::parse_userdata(const LuaMadeSimple::Lua &lua, const std::string& funcName) {
    if (!lua.is_userdata())
        lua.throw_error(std::format("Function '{}' must be called as a member function", funcName));
    return lua.get_userdata<T>();
}

template<typename T>
std::pair<const T&, std::string> LuaStaticsSDF::parse_string_arg(const LuaMadeSimple::Lua &lua, const std::string& funcName) {
    const auto& lua_object = parse_userdata<T>(lua, funcName);

    if (!lua.is_string())
        lua.throw_error(std::format("Parameter #1 for function '{}' must be a string", funcName));

    return {lua_object, std::string(lua.get_string())};
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
