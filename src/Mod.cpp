
#include "parsing/FileTraversal.hpp"
#include "parsing/BuilderActionParser.hpp"
#include "parsing/CategoryParser.hpp"
#include "parsing/ItemTypeParser.hpp"
#include "parsing/RecipeParser.hpp"
#include "parsing/StoryGoalParser.hpp"
#include "parsing/DatabankEntryParser.hpp"

#include "util/Hooks.hpp"

#include "api/cpp/SDF.hpp"
#include "api/lua/SDF.hpp"
#include "Mod/CppUserModBase.hpp"
#include "FWorldContext.hpp"

#include "SDF/Version.hpp"
#include "util/Log.hpp"

using namespace RC;
using namespace Unreal;

class SDF_Mod final : public CppUserModBase {
    static void startup();

public:
    SDF_Mod();
    ~SDF_Mod() override;

    auto on_unreal_init() -> void override;
    auto on_lua_start(StringViewType mod_name, LuaMadeSimple::Lua &lua, LuaMadeSimple::Lua &main_lua, LuaMadeSimple::Lua &async_lua, LuaMadeSimple::Lua *hook_lua) -> void override;
};

#define MOD_EXPORT __declspec(dllexport)
extern "C" {
MOD_EXPORT inline CppUserModBase* start_mod(){ return new SDF_Mod(); }
MOD_EXPORT inline void uninstall_mod(const CppUserModBase* mod) { delete mod; }
}

void SDF_Mod::on_unreal_init() {
    const Hook::FCallbackOptions common_opts {true, false, STR("SDF"), STR("SDF_Startup")};
    Hook::RegisterLoadMapPostCallback([this](Hook::TCallbackIterationData<bool>&, UEngine*, FWorldContext&, FURL, UPendingNetGame*, FString&) {
        startup();
    }, common_opts);
}

void SDF_Mod::startup() {
    Log::Verbose("SDF Version {} Initialized", SDFModVersion);

    Hooks::RegisterHooks();

    SDF_Impl::TriggerEvent(SDF::Event::PreTraverse);
    FileTraversal::ScanFiles();
    SDF_Impl::TriggerEvent(SDF::Event::PostTraverse);

    StoryGoalParser::ParseStoryGoals();
    //ItemTypeParser::ParseItemTypes();

    SDF_Impl::TriggerEvent(SDF::Event::PreCategory);
    CategoryParser::ParseCategories();
    SDF_Impl::TriggerEvent(SDF::Event::PostCategory);

    SDF_Impl::TriggerEvent(SDF::Event::PreRecipe);
    //RecipeParser::ParseRecipes();
    SDF_Impl::TriggerEvent(SDF::Event::PostRecipe);

    //BuilderActionParser::ParseBuilderActions();

    SDF_Impl::TriggerEvent(SDF::Event::PreDatabankEntry);
    //DatabankEntryParser::ParseDatabankEntries();
    SDF_Impl::TriggerEvent(SDF::Event::PostDatabankEntry);

#ifdef DEVELOPMENT
    BioModFactory bmf("CustomBioMod", false);
    bmf.setName("Test Bio Mod");
    bmf.setDescription("This bio mod is for testing the ability of SDF to create custom bio mods");
    bmf.setType(SDK::EUWEBioAbilityType::Passive);
    if (!bmf.registerBioMod())
        Log::Warning("Failed to register test biomod");
#endif
}

SDF_Mod::SDF_Mod() {
    ModVersion = W(SDFModVersion);
    ModName = L"Subnautica Data Framework";
    ModAuthors = L"Limo";
    ModDescription = L"A Subnautica 2 framework that allows for data driven content mods";
}

SDF_Mod::~SDF_Mod() {
    Hooks::UnregisterHooks();
}

void SDF_Mod::on_lua_start(StringViewType mod_name, LuaMadeSimple::Lua &lua, LuaMadeSimple::Lua &main_lua, LuaMadeSimple::Lua &async_lua, LuaMadeSimple::Lua *hook_lua) {
#ifdef DEVELOPMENT
    SDF_Lua::RegisterLuaTypes(lua);
#endif
}
