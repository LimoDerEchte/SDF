#include "registering/Hooks.hpp"
#include "registering/RecipeFactory.hpp"

#include <DynamicOutput/Output.hpp>
#include <Mod/CppUserModBase.hpp>

#include "registering/CategoryFactory.hpp"
#include "util/Log.hpp"

using namespace RC;
using namespace Unreal;

class SN2CustomCraft : public CppUserModBase {
    bool scanning = true;
public:
    SN2CustomCraft() {
        ModVersion = STR("0.1");
        ModName = STR("SN2CustomCraft");
        ModAuthors = STR("Limo");
        ModDescription = STR("A utility mod that allows anyone to modify the games crafting recipes");

        Log::Verbose("Initialized");
    }
    ~SN2CustomCraft() override = default;

    auto on_update() -> void override {
        if (!scanning)
            return;
        if (const auto world = SDK::UWorld::GetWorld()) {
            if (world->GetName().contains("ClientLobby")) {
                scanning = false;
                Startup();
            }
        }
    }

    static void Startup() {
        Hooks::RegisterHooks();

        CategoryFactory upperCategory("UpperCategory", "Upper Category", "This is an upper category", ECrafterType::Fabricator);
        upperCategory.setIconFromItem("MetalSalvage");
        const auto upperCatPtr = upperCategory.registerCategory();

        CategoryFactory category("CustomCategory", "Custom Category", "This is a custom category :)", ECrafterType::Fabricator);
        category.setParent("Sustenance");
        category.setIconFromItem("CopperIngot");
        const auto catPtr = category.registerCategory();
        //catPtr->OrderingIndex = 50;

        RecipeFactory recipe("CustomRecipe", "Copper Ingot", "This is a custom test recipe :)");
        recipe.setIconFromItem("CopperIngot");
        recipe.setCategory(catPtr);

        recipe.addIngredient("PowerCell", 1);
        recipe.addIngredient("Grease", 3);
        recipe.addOutput("CopperIngot", 2);

        recipe.registerRecipe();
    }
};


#define MOD_EXPORT __declspec(dllexport)
extern "C" {
MOD_EXPORT CppUserModBase* start_mod(){ return new SN2CustomCraft(); }
MOD_EXPORT void uninstall_mod(const CppUserModBase* mod) { delete mod; }
}
