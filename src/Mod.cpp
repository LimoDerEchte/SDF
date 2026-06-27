
#include "registering/BioModFactory.hpp"
#include "parsing/FileTraversal.hpp"
#include "parsing/BuilderActionParser.hpp"
#include "parsing/CategoryParser.hpp"
#include "parsing/ItemTypeParser.hpp"
#include "parsing/RecipeParser.hpp"
#include "parsing/StoryGoalParser.hpp"
#include "parsing/DatabankEntryParser.hpp"

#include "util/Hooks.hpp"

#include "SDF/Version.hpp"
#include "util/Log.hpp"

#include "api/cpp/SDF.hpp"
#include "api/lua/SDF.hpp"
#include "Mod/CppUserModBase.hpp"

using namespace RC;
using namespace Unreal;

class SN2CustomCraft : public CppUserModBase {
    bool scanning = true;

    static void startup();

public:
    SN2CustomCraft();
    ~SN2CustomCraft() override;

    auto on_update() -> void override;
    auto on_lua_start(LuaMadeSimple::Lua &lua, LuaMadeSimple::Lua &main_lua, LuaMadeSimple::Lua &async_lua, LuaMadeSimple::Lua *hook_lua) -> void override;
};

#define MOD_EXPORT __declspec(dllexport)
extern "C" {
MOD_EXPORT inline CppUserModBase* start_mod(){ return new SN2CustomCraft(); }
MOD_EXPORT inline void uninstall_mod(const CppUserModBase* mod) { delete mod; }
}

void SN2CustomCraft::startup() {
    Log::Verbose("SDF Version {} Initialized", SDFModVersion);

    Hooks::RegisterHooks();

    FileTraversal::ScanFiles();
    StoryGoalParser::ParseStoryGoals();
    ItemTypeParser::ParseItemTypes();

    SDF_Impl::TriggerEvent(SDF::EventPreCategory);
    CategoryParser::ParseCategories();
    SDF_Impl::TriggerEvent(SDF::EventPostCategory);

    SDF_Impl::TriggerEvent(SDF::EventPreRecipe);
    RecipeParser::ParseRecipes();
    SDF_Impl::TriggerEvent(SDF::EventPostRecipe);

    BuilderActionParser::ParseBuilderActions();
    DatabankEntryParser::ParseDatabankEntries();

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

    BioModFactory bmf("CustomBioMod", false);
    bmf.setName("Test Bio Mod");
    bmf.setDescription("This bio mod is for testing the ability of SDF to create custom bio mods");
    bmf.setType(SDK::EUWEBioAbilityType::Passive);
    if (!bmf.registerBioMod())
        Log::Warning("Failed to register test biomod");
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
    SDF_Lua::RegisterLuaTypes(lua, main_lua, async_lua, hook_lua);
    Log::Verbose("Lua API initialized");
}
