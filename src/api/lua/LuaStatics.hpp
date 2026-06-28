//
// Created by Limo on 28/06/2026.
//

#pragma once

#include "Mod/LuaMod.hpp"

class LuaStaticsSDF {
public:
    static int ensure_hook_thread_exists(LuaMod* mod);
    static std::pair<LuaMadeSimple::Lua*, int> make_hook_state(LuaMod* mod);
};

class LuaType {
    const LuaMadeSimple::Lua &lua;

public:
    explicit LuaType(const LuaMadeSimple::Lua &lua);

    void add_table(const std::string &key, const std::function<void(const LuaMadeSimple::Lua::Table &table)> &adder) const;
    void add_function(const std::string &key, const LuaMadeSimple::Lua::LuaFunction &function) const;

    void make_global(const std::string &key) const;
};
