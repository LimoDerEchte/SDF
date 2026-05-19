#include <DynamicOutput/Output.hpp>
#include <Mod/CppUserModBase.hpp>
#include <Unreal/UObjectGlobals.hpp>
#include <Unreal/UObject.hpp>

namespace MyMods
{
    using namespace RC;
    using namespace Unreal;

    class MyExampleMod : public RC::CppUserModBase {
    public:
        MyExampleMod() {
            ModVersion = STR("0.1");
            ModName = STR("SN2CustomCraft");
            ModAuthors = STR("Limo");
            ModDescription = STR("A utility mod that allows anyone to modify the games crafting recipes");

            Output::send<LogLevel::Warning>(STR("[MyExampleMod]: Init.\n"));
        }
        ~MyExampleMod() = default;

        auto on_unreal_init() -> void override
        {
        }
    };
}

#define MOD_EXPORT __declspec(dllexport)
extern "C" {
MOD_EXPORT RC::CppUserModBase* start_mod(){ return new MyMods::MyExampleMod(); }
MOD_EXPORT void uninstall_mod(RC::CppUserModBase* mod) { delete mod; }
}
