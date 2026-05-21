//
// Created by Limo on 20/05/2026.
//

#include "Hooks.hpp"

#include "UnrealDef.hpp"
#include "UObjectGlobals.hpp"
#include "DynamicOutput/Output.hpp"
#include "SDK/Basic.hpp"
#include "polyhook2/Exceptions/AVehHook.hpp"

getRecipeT Hooks::originalGetRecipes = nullptr;

std::unique_ptr<PLH::Detour> Hooks::getRecipesHook = nullptr;

UC::TArray<SDK::UUWECraftingRecipe *> Hooks::GetRecipesHook() {
    RC::Output::send<RC::LogLevel::Verbose>(STR("[SN2CustomCraft]: Get recipes called!\n"));
    return originalGetRecipes();
}

uintptr_t Hooks::ScanCall(uintptr_t address, int ordinal) {
    while (true) {
        const auto byte = reinterpret_cast<uint8_t*>(address);
        if (*byte == 0xE8) {
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

void Hooks::RegisterHooks() {
    RC::Output::send<RC::LogLevel::Verbose>(L"[SN2CustomCraft]: Hooking recipe registry\n");

    const auto recipeFunc = RC::UObjectGlobals::FindObject<RC::UFunction>(nullptr, L"/Script/Subnautica2.SN2AssetRegistry:GetAllCraftingRecipes");
    const auto funcPtr = reinterpret_cast<uintptr_t>(*recipeFunc->GetFuncPtr());
    const auto internalPtr = ScanCall(funcPtr, 1);

    wchar_t buf[256];
    swprintf(buf,256,L"[SN2CustomCraft]: Found recipe registry getter at %p\n", (void*)internalPtr);
    RC::Output::send<RC::LogLevel::Verbose>(buf);

    getRecipesHook.reset(new PLH::x64Detour(
        internalPtr,
        reinterpret_cast<uint64_t>(&GetRecipesHook),
        reinterpret_cast<uint64_t*>(&originalGetRecipes)
    ));

    if (!getRecipesHook->hook())
        RC::Output::send<RC::LogLevel::Error>(L"[SN2CustomCraft]: Failed to hook recipe registry!\n");
}
