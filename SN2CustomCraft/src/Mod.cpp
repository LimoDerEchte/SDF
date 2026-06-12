
#include "Mod.hpp"

#include <DynamicOutput/Output.hpp>
#include <Mod/CppUserModBase.hpp>

#include "UnrealDef.hpp"
#include "SDF/Version.hpp"

#include "parsing/CategoryParser.hpp"
#include "parsing/FileTraversal.hpp"
#include "parsing/RecipeParser.hpp"

#include "registering/Hooks.hpp"
#include "registering/RecipeFactory.hpp"
#include "registering/BuilderActionFactory.hpp"
#include "registering/ItemTypeFactory.hpp"

#include "util/IconBuilder.hpp"
#include "util/Log.hpp"

using namespace RC;
using namespace Unreal;

void SN2CustomCraft::startup() {
    Log::Verbose("SDF Version {} Initialized", SDFModVersion);

    Hooks::RegisterHooks();

#ifdef DEVELOPMENT
    ItemTypeFactory type("CustomItem", "Custom Item Type", "This is a custom item type because why tf not");
    type.setIconFromItem("CopperIngot");
    UUWEPrimaryDataAssetBase* item = type.registerItemType();
    if (!item)
        Log::Error("What in the skibidi");
#endif

    FileTraversal::ScanFiles();
    CategoryParser::ParseCategories();
    RecipeParser::ParseRecipes();

#ifdef DEVELOPMENT
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

    BuilderActionFactory factory("CustomBuilderRecipeRecipe");
    //factory.addPowerDrainText("-1 Aura");
    if (!factory.registerBuilderAction())
        Log::Error("Errm what the sigma");

    Log::Warning("Finding Transient UWEAssetRegistrySubsystem");

    std::vector<Unreal::UObject*> list{};
    RC::UObjectGlobals::FindAllOf(L"UWEAssetRegistrySubsystem", list);

    // Research
    USN2AssetRegistry::RebuildAssetRegistryCachedData();

    std::vector<Unreal::UObject*> list2{};
    UObjectGlobals::FindAllOf(L"SN2FabricatorViewModel", list2);

    for (const auto& obj : list2) {
        if (!obj->GetFullName().contains(L"/Engine/Transient."))
            continue;

        const auto test = reinterpret_cast<USN2FabricatorViewModel*>(obj);
        for (const auto entry : test->CategoryViewModelsByCategory) {
            Log::Warning("Test {}", entry.Second->GetCrafterText().ToString());
        }
    }
#endif
}

SN2CustomCraft::SN2CustomCraft() {
    ModVersion = W(SDFModVersion);
    ModName = L"Subnautica Data Framework";
    ModAuthors = L"Limo";
    ModDescription = L"A Subnautica 2 framework that allows for data driven content mods";
}

SN2CustomCraft::~SN2CustomCraft() {
    if (scanning)
        return;
    Hooks::UnregisterHooks();

    // TODO: RecipeFactory::unregisterAllRecipes();
    // TODO: CategoryFactory::unregisterAllCategories();
    FileTraversal::DeleteCache();
}

void SN2CustomCraft::on_update() {
    if (!scanning)
        return;
    if (const auto world = SDK::UWorld::GetWorld()) {
        if (world->GetName().contains("ClientLobby")) {
            scanning = false;
            startup();
        }
    }
}

void SN2CustomCraft::on_lua_start(LuaMadeSimple::Lua &lua, LuaMadeSimple::Lua &main_lua, LuaMadeSimple::Lua &async_lua, LuaMadeSimple::Lua *hook_lua) {
    // TODO: Future lua API will go here
}
