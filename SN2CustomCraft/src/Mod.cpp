#include "registering/Hooks.hpp"

#include <DynamicOutput/Output.hpp>
#include <Mod/CppUserModBase.hpp>

using namespace RC;
using namespace Unreal;

class SN2CustomCraft : public CppUserModBase {
public:
    SN2CustomCraft() {
        ModVersion = STR("0.1");
        ModName = STR("SN2CustomCraft");
        ModAuthors = STR("Limo");
        ModDescription = STR("A utility mod that allows anyone to modify the games crafting recipes");

        Output::send<LogLevel::Verbose>(STR("[SN2CustomCraft]: Initialized\n"));
    }
    ~SN2CustomCraft() override = default;

    auto on_unreal_init() -> void override
    {
        Hooks::RegisterHooks();
    }
};


#define MOD_EXPORT __declspec(dllexport)
extern "C" {
MOD_EXPORT CppUserModBase* start_mod(){ return new SN2CustomCraft(); }
MOD_EXPORT void uninstall_mod(const CppUserModBase* mod) { delete mod; }
}
