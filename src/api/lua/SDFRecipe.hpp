//
// Created by Limo on 28/06/2026.
//

#pragma once

#include "LuaStatics.hpp"
#include "LuaMadeSimple/LuaObject.hpp"
#include "SDF/SDFRecipe.hpp"

class SDFRecipe_Lua : RC::LuaMadeSimple::Type::BaseObject {
    static int setName(const RC::LuaMadeSimple::Lua &lua);
    static int setDescription(const RC::LuaMadeSimple::Lua &lua);

    static int setCategory(const RC::LuaMadeSimple::Lua &lua);
    static int setIcon(const RC::LuaMadeSimple::Lua &lua);

    static int addIngredient(const RC::LuaMadeSimple::Lua &lua);
    static int addOutput(const RC::LuaMadeSimple::Lua &lua);

    static int addItemPickupUnlockingRequirement(const RC::LuaMadeSimple::Lua &lua);
    static int addScanDataUnlockingRequirement(const RC::LuaMadeSimple::Lua &lua);
    static int removeUnlockingRequirements(const RC::LuaMadeSimple::Lua &lua);

    static int setCraftingTime(const RC::LuaMadeSimple::Lua &lua);
    static int setAvailableInLifepod(const RC::LuaMadeSimple::Lua &lua);
    static int setOrderingIndex(const RC::LuaMadeSimple::Lua &lua);

    static int createOrModifyRecipe(const RC::LuaMadeSimple::Lua &lua);

    GenericObjectHeader(SDFRecipe)
};
