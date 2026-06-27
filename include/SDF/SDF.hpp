//
// Created by Limo on 27/06/2026.
//

#pragma once

#include <Windows.h>
#include <UObject.hpp>

class SDF {
    typedef SDF*(*GetterFunc)();
    static SDF* instance;

public:
    enum AssetType {
        Recipe,
        Category
    };

    enum Event {
        EventPreRecipe,
        EventPostRecipe,
        EventPreCategory,
        EventPostCategory,
    };

    virtual ~SDF() = default;

    typedef void(*EventCallback)(Event event);
    typedef void(*CreateAssetCallback)(AssetType type, const std::string& id, RC::Unreal::UObject* asset);

    virtual uint64_t HookEvent(EventCallback callback);
    virtual uint64_t HookCreateAsset(CreateAssetCallback callback);
    virtual void Unhook(uint64_t hookId);

    static SDF* Get() {
        if (instance == nullptr) {
            const HMODULE mod = GetModuleHandleA("SDF.dll");
            if (mod == nullptr)
                return nullptr;
            const auto func = reinterpret_cast<GetterFunc>(GetProcAddress(mod, "sdf_get_sdf"));
            if (func == nullptr)
                return nullptr;
            instance = func();
        }
        return instance;
    }
};
