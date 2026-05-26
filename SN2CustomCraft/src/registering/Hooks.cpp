//
// Created by Limo on 20/05/2026.
//

#include "Hooks.hpp"

#include <memory>

#include "util/Log.hpp"
#include "RecipeFactory.hpp"
#include "polyhook2/Exceptions/AVehHook.hpp"
#include "SDK/Subnautica2_classes.hpp"
#include "Hooks/Hooks.hpp"

#include "BuilderActionFactory.hpp"
#include "ItemTypeFactory.hpp"
#include "UnrealDef.hpp"

using namespace SDK;
using namespace RC;
using namespace Unreal;

#define HookDefStatic(type, name, list) \
    Hooks::get##name##T Hooks::originalGet##name = nullptr; \
    std::unique_ptr<PLH::Detour> Hooks::get##name##Hook = nullptr; \
    \
    Unreal::TArray<type*> Hooks::Get##name##Hook() { \
        auto entries = originalGet##name(); \
        entries.ResizeTo(entries.Num() + static_cast<int32_t>(list.size())); \
        \
        for (const type* entry : list) { \
            entries.Add(const_cast<type*>(entry)); \
        } \
        return entries; \
    }

HookDefStatic(UUWECraftingRecipe, Recipes, RecipeFactory::registeredRecipes)
HookDefStatic(USN2BuilderActionData, BuilderActions, BuilderActionFactory::registeredActions)
HookDefStatic(UUWEItemType, ItemTypes, ItemTypeFactory::registeredItemTypes)

uintptr_t Hooks::ScanCall(uintptr_t address, int ordinal) {
    while (true) {
        if (*reinterpret_cast<uint8_t*>(address) == 0xE8) {
            ordinal--;
            if (ordinal > 0) {
                address += 5;
                continue;
            }
            const int32_t rel = *reinterpret_cast<int32_t*>(address + 1);
            return address + rel + 5;
        }
        address++;
    }
}

#define HookDefScanAndHook(name, funcName) \
    const auto funcGet##name = USN2AssetRegistry::StaticClass()->GetFunction("SN2AssetRegistry", funcName); \
    const auto funcPtrGet##name = reinterpret_cast<uintptr_t>(*funcGet##name->ExecFunction); \
    const auto internalPtrGet##name = ScanCall(funcPtrGet##name, 1); \
    Log::Verbose("Found "#name" registry getter at {:p}", reinterpret_cast<void*>(internalPtrGet##name)); \
    \
    get##name##Hook = std::make_unique<PLH::x64Detour>( \
        internalPtrGet##name, \
        reinterpret_cast<uint64_t>(&Get##name##Hook), \
        reinterpret_cast<uint64_t*>(&originalGet##name) \
    ); \
    \
    if (get##name##Hook->hook()) \
        Log::Verbose("Successfully hooked "#name" registry getter"); \
    else \
        Log::Error("Failed to hook "#name" registry getter!");

void Hooks::RegisterHooks() {
    Log::Verbose("Scanning internal functions");

    HookDefScanAndHook(Recipes, "GetAllCraftingRecipes");
    HookDefScanAndHook(BuilderActions, "GetAllBuilderActions");
    HookDefScanAndHook(ItemTypes, "GetAllItemTypes");
}

#define HookDefUnhook(name) \
    if (get##name##Hook->unHook()) \
        Log::Verbose("Successfully unhooked "#name" registry getter"); \
    else \
        Log::Error("Failed to unhook "#name" registry getter!");

void Hooks::UnregisterHooks() {
    HookDefUnhook(Recipes);
    HookDefUnhook(BuilderActions);
    HookDefUnhook(ItemTypes);
}
