//
// Created by Limo on 20/05/2026.
//

#include "registering/Hooks.hpp"

#include <memory>

#include "DatabankEntryFactory.hpp"
#include "util/Log.hpp"
#include "registering/RecipeFactory.hpp"
#include "polyhook2/Exceptions/AVehHook.hpp"
#include "SDK/Subnautica2_classes.hpp"
#include "registering/BuilderActionFactory.hpp"
#include "registering/ItemTypeFactory.hpp"
#include "UnrealDef.hpp"
#include "LuaMadeSimple/LuaMadeSimple.hpp"
#include "SDK/AssetRegistry_classes.hpp"

using namespace SDK;
using namespace RC;
using namespace Unreal;

#define HookDefStatic(type, name, list, message) \
    Hooks::get##name##T Hooks::originalGet##name = nullptr; \
    std::unique_ptr<PLH::Detour> Hooks::get##name##Hook = nullptr; \
    \
    SDK::TArray<type*> Hooks::Get##name##Hook() { \
        auto originalEntries = originalGet##name(); \
        const auto entries = reinterpret_cast<Unreal::TArray<type*>*>(&originalEntries); \
        entries->ResizeTo(entries->Num() + static_cast<int32_t>(list.size())); \
        \
        if(message) \
            Log::Warning("{} {}", #name, entries->Num()); \
        \
        for (const type* entry : list) { \
            entries->Add(const_cast<type*>(entry)); \
        } \
        return originalEntries; \
    }

HookDefStatic(UUWECraftingRecipe, Recipes, RecipeFactory::registeredRecipes, false)
HookDefStatic(USN2BuilderActionData, BuilderActions, BuilderActionFactory::registeredActions, false)
HookDefStatic(UUWEDatabankEntry, DatabankEntries, DatabankEntryFactory::registeredDatabankEntries, false)

#ifdef DEVELOPMENT
Hooks::getAssetsT Hooks::originalGetAssets = nullptr;
std::unique_ptr<PLH::Detour> Hooks::getAssetsHook = nullptr;

bool Hooks::GetAssetsHook(void* self, const FARFilter *filter, SDK::TArray<SDK::FAssetData> *out, const bool bSkipARFilteredAsset) {
    bool ret = originalGetAssets(self, filter, out, bSkipARFilteredAsset);

    if (out)
        Log::Warning("Result count: {}", out->Num());
    else
        Log::Warning("Invalid result array");

    //for (const auto name : filter->ClassNames)
    //    Log::Warning("Scanning for classname: {}", name.ToString());

    for (const auto asset : filter->ClassPaths) {
        Log::Warning("Scanning for classpath: {} {}", asset.PackageName.ToString(), asset.AssetName.ToString());
    }

    for (const auto name : filter->PackageNames)
        Log::Warning("Scanning for packagename: {}", name.ToString());

    for (const auto name : filter->PackagePaths)
        Log::Warning("Scanning for packagepath: {}", name.ToString());

    return ret;
}
#endif

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

uintptr_t Hooks::ScanCallMultiPass(uintptr_t address, const std::vector<int> &ordinals) {
    for (const int ordinal : ordinals) {
        address = ScanCall(address, ordinal);
    }
    return address;
}


#define HookDefScan(name, funcName, ...) \
    const auto funcGet##name = USN2AssetRegistry::StaticClass()->GetFunction("SN2AssetRegistry", funcName); \
    const auto funcPtrGet##name = reinterpret_cast<uintptr_t>(*funcGet##name->ExecFunction); \
    const auto internalPtrGet##name = ScanCallMultiPass(funcPtrGet##name, std::vector{__VA_ARGS__}); \
    Log::Verbose("Found "#name" registry getter at {:p} ({:p})", reinterpret_cast<void*>(internalPtrGet##name), reinterpret_cast<void*>(internalPtrGet##name - moduleBase)); \

#define HookDefHook(name) \
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

#define HookDefUnhook(name) \
    if (get##name##Hook->unHook()) \
        Log::Verbose("Successfully unhooked "#name" registry getter"); \
    else \
        Log::Error("Failed to unhook "#name" registry getter!");

void Hooks::RegisterHooks() {
    Log::Verbose("Scanning internal functions");

    const auto moduleBase = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));

    HookDefScan(Recipes, "GetAllCraftingRecipes", 1);
    HookDefScan(BuilderActions, "GetAllBuilderActions", 1);
    HookDefScan(DatabankEntries, "GetAllDatabankEntries", 1);

#ifdef DEVELOPMENT
    HookDefScan(ItemTypes, "GetAllItemTypes", 1);

    const auto assetRegistryVTable = *static_cast<uintptr_t**>(SDK::UAssetRegistryHelpers::GetAssetRegistry().InterfacePointer);

    const auto internalPtrGetAssets = assetRegistryVTable[10];
    Log::Verbose("Found Assets registry getter at {:p} ({:p})", reinterpret_cast<void*>(internalPtrGetAssets), reinterpret_cast<void*>(internalPtrGetAssets - moduleBase));
#endif

    HookDefHook(Recipes);
    HookDefHook(BuilderActions);
    HookDefHook(DatabankEntries);

#ifdef DEVELOPMENT
    //HookDefHook(Assets);
#endif
}

void Hooks::UnregisterHooks() {
    HookDefUnhook(Recipes);
    HookDefUnhook(BuilderActions);
    HookDefUnhook(DatabankEntries);

#ifdef DEVELOPMENT
    //HookDefUnhook(Assets);
#endif
}
