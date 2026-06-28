//
// Created by Limo on 28/06/2026.
//

#pragma once

#include "LuaMadeSimple/LuaObject.hpp"
#include "SDF/SDFRecipe.hpp"

class SDFRecipe_Lua : RC::LuaMadeSimple::Type::BaseObject {
    std::unique_ptr<SDFRecipe> ref;

    static int setName(const RC::LuaMadeSimple::Lua &lua);
    static int setDescription(const RC::LuaMadeSimple::Lua &lua);
    static int createOrModifyRecipe(const RC::LuaMadeSimple::Lua &lua);

public:
    explicit SDFRecipe_Lua(std::unique_ptr<SDFRecipe> &&impl);

    static auto construct(const RC::LuaMadeSimple::Lua &lua,
                          std::unique_ptr<SDFRecipe> &&impl) -> RC::LuaMadeSimple::Lua::Table;

    static void setupFunctions(const RC::LuaMadeSimple::Lua::Table &table);
};
