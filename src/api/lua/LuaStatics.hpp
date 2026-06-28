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
