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
