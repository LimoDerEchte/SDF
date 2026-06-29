//
// Created by Limo on 28/06/2026.
//

#pragma once

#include "Mod/LuaMod.hpp"

#define GenericObjectHeader(typeName) \
    std::unique_ptr<typeName> ref; \
public: \
    explicit typeName##_Lua(std::unique_ptr<typeName> &&ref); \
    static auto construct(const RC::LuaMadeSimple::Lua &lua, std::unique_ptr<typeName> &&ref) -> RC::LuaMadeSimple::Lua::Table; \
    static void setupFunctions(const RC::LuaMadeSimple::Lua::Table &table);

#define GenericObjectSource(typeName) \
using namespace RC; \
using namespace LuaMadeSimple; \
\
typeName##_Lua::typeName##_Lua(std::unique_ptr<typeName> &&ref) : BaseObject(#typeName), ref(std::move(ref)) { } \
\
auto typeName##_Lua::construct(const Lua &lua, std::unique_ptr<typeName> &&ref) -> Lua::Table { \
    typeName##_Lua object{std::move(ref)}; \
    const Lua::Table table = lua.get_metatable(#typeName); \
    if (lua.is_nil(-1)) { \
        lua.discard_value(-1); \
        lua_createtable(lua.get_lua_state(), 0, 0); \
        setupFunctions(table); \
        lua.new_metatable<typeName##_Lua>(#typeName, object.get_metamethods()); \
    } \
    lua.transfer_stack_object(std::move(object), #typeName, object.get_metamethods()); \
    return table; \
}

#define GenericTypeFunction(typeName) \
    table.add_pair("type", [](const Lua& lua) { \
        lua.set_string(#typeName); \
        return 1; \
    });


class LuaStaticsSDF {
public:
    static int ensure_hook_thread_exists(LuaMod* mod);
    static std::pair<LuaMadeSimple::Lua*, int> make_hook_state(LuaMod* mod);

    template<typename T>
    static const T &parse_userdata(const LuaMadeSimple::Lua &lua, const std::string& funcName) {
        if (!lua.is_userdata())
            lua.throw_error(std::format("Function '{}' must be called as a member function", funcName));
        return lua.get_userdata<T>();
    }

    template<typename T>
    static std::pair<const T&, std::string> parse_string_arg(const LuaMadeSimple::Lua &lua, const std::string& funcName) {
        const auto& lua_object = parse_userdata<T>(lua, funcName);

        if (!lua.is_string())
            lua.throw_error(std::format("Parameter #1 for function '{}' must be a string", funcName));

        return {lua_object, std::string(lua.get_string())};
    }

    template<typename T>
    static std::pair<const T&, std::variant<std::string, Unreal::UObject*>> parse_string_or_object_arg(const LuaMadeSimple::Lua &lua, const std::string& funcName) {
        const auto& lua_object = parse_userdata<T>(lua, funcName);

        if (lua.is_string())
            return {lua_object, std::string(lua.get_string())};

        // TODO: Figure out object read

        lua.throw_error(std::format("Parameter #1 for function '{}' must be a string or UObject", funcName));
        throw new std::runtime_error("Unreachable");
    }
};

class LuaTypeFactory {
    const LuaMadeSimple::Lua &lua;

public:
    explicit LuaTypeFactory(const LuaMadeSimple::Lua &lua);

    void add_enum(const std::string &key, const std::map<std::string, int> &entries) const;
    void add_function(const std::string &key, const LuaMadeSimple::Lua::LuaFunction &function) const;

    void make_global(const std::string &key) const;
};
