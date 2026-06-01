#include "registering/Hooks.hpp"
#include "registering/RecipeFactory.hpp"

#include <DynamicOutput/Output.hpp>
#include <Mod/CppUserModBase.hpp>

#include "UnrealDef.hpp"
#include "parsing/CategoryParser.hpp"
#include "parsing/FileTraversal.hpp"
#include "parsing/RecipeParser.hpp"
#include "registering/BuilderActionFactory.hpp"
#include "registering/ItemTypeFactory.hpp"
#include "util/IconBuilder.hpp"
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
    ~SN2CustomCraft() override {
        if (scanning)
            return;
        Hooks::UnregisterHooks();

        // TODO: RecipeFactory::unregisterAllRecipes();
        // TODO: CategoryFactory::unregisterAllCategories();
        FileTraversal::DeleteCache();
    }

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

        ItemTypeFactory type("CustomItem", "Custom Item Type", "This is a custom item type because why tf not");
        type.setIconFromItem("CopperIngot");
        UUWEPrimaryDataAssetBase* item = type.registerItemType();
        if (!item)
            Log::Error("What in the skibidi");

        FileTraversal::ScanFiles();
        CategoryParser::ParseCategories();
        RecipeParser::ParseRecipes();

        //RecipeFactory recipe("TestRec", "Test Recipe", "This is a recipe for testing dynamic icons");
        //recipe.setCategory("CustomCategory");
        //recipe.addIngredient("Titanium", 2);
        //recipe.addOutput("Copper", 1);

        //IconBuilder icon{};
        //icon.addIconFromItem("Copper");
        //icon.addIconFromItem("Titanium", StepData {
        //    .x = 10, .y = 10, .width = 64, .height = 64
        //});
        //recipe.setIcon(icon.build());
        //const auto _ = recipe.registerRecipe();

        BuilderActionFactory factory("CustomBuilderRecipe");
        factory.addPowerDrainText("-1 Aura");
        if (!factory.registerBuilderAction())
            Log::Error("Errm what the sigma");

        Log::Warning("Finding Transient UWEAssetRegistrySubsystem");

        std::vector<Unreal::UObject*> list{};
        RC::UObjectGlobals::FindAllOf(L"UWEAssetRegistrySubsystem", list);

        for (const auto& i : list) {
            if (!i->GetFullName().contains(L"/Engine/Transient.GameEngine"))
                continue;

            const auto obj = reinterpret_cast<UUWEAssetRegistrySubsystem*>(i);

            Log::Warning("Test 1: {}", obj->CachedAssetClasses.Num());
            Log::Warning("Test 2: {}", obj->CachedAssets.Num());
            Log::Warning("Test 3: {}", obj->CachedBlueprintClasses.Num());
            Log::Warning("Test 4: {}", obj->CachedDataAssets.Num());

            for (auto& pair : obj->CachedDataAssets) {
                if (!pair.First->GetFullName().contains("ItemType"))
                    continue;
                Log::Warning("Item Types: {}", pair.Second.Assets.Num());

                auto* types = reinterpret_cast<Unreal::TArray<UUWEPrimaryDataAssetBase*>*>(&pair.Second.Assets);
                types->Add(item);
                Log::Warning("Item type inserted");
            }
        }
    }
};


#define MOD_EXPORT __declspec(dllexport)
extern "C" {
MOD_EXPORT CppUserModBase* start_mod(){ return new SN2CustomCraft(); }
MOD_EXPORT void uninstall_mod(const CppUserModBase* mod) { delete mod; }
}
