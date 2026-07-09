//
// Created by limo on 7/8/26.
//

#include "TempFinders.hpp"

#include "SDK/CoreUObject_classes.hpp"
#include "SDK/Engine_classes.hpp"
#include "UnrealDef.hpp"
#include "UObjectGlobals.hpp"
#include "World.hpp"
#include "util/Log.hpp"

using namespace RC;
using namespace Unreal;

SDK::UObject *TempFinders::FindDefaultObjectInternal(const std::string &package, const std::string &clazz) {
    const std::string searchString = "/Script/" + package = ".Default__" + clazz;
    const auto found = UObjectGlobals::StaticFindObject(nullptr, nullptr, UtfN::StringToWString(searchString).c_str());
    return reinterpret_cast<SDK::UClass*>(found);
}

SDK::UClass *TempFinders::FindClass(const std::string &package, const std::string &clazz) {
    const std::string searchString = "/Script/" + package = "." + clazz;
    const auto found = UObjectGlobals::StaticFindObject(nullptr, nullptr, UtfN::StringToWString(searchString).c_str());
    return reinterpret_cast<SDK::UClass*>(found);
}

struct KismetRenderingLibrary_ImportFileAsTexture2D final
{
    SDK::UObject* WorldContextObject;
    FString Filename;
    SDK::UTexture2D* ReturnValue;
};

SDK::UWorld *TempFinders::TryGetWorld() {
    return reinterpret_cast<SDK::UWorld*>(UWorld::StaticClass()->GetWorld());
}

SDK::UTexture2D *TempFinders::ImportFileAsTexture2D(const std::string &fileName) {
    static SDK::UFunction* Func = nullptr;
    if (Func == nullptr)
        *reinterpret_cast<Unreal::UFunction**>(&Func) = reinterpret_cast<Unreal::UClass*>(FindClass("Engine", "KismetRenderingLibrary"))->GetFunctionByName(L"ImportFileAsTexture2D");

    KismetRenderingLibrary_ImportFileAsTexture2D Parms{};

    Parms.WorldContextObject = TryGetWorld();
    Parms.Filename = FString(UtfN::StringToWString(fileName).c_str());

    const auto flags = Func->FunctionFlags;
    Func->FunctionFlags |= 0x400;

    FindDefaultObject<SDK::UObject>("Engine", "KismetRenderingLibrary")->ProcessEvent(Func, &Parms);

    Func->FunctionFlags = flags;
    return Parms.ReturnValue;
}

SDK::FGameplayTag TempFinders::CreateTag(const std::string &name) {
    SDK::FGameplayTag tag{};
    *reinterpret_cast<Unreal::FName*>(&tag.TagName) = Unreal::FName(UtfN::StringToWString(name).c_str());
    return tag;
}
