//
// Created by Limo on 27/06/2026.
//

#pragma once

#include <UObject.hpp>

#include <functional>
#include <string>

#include <Windows.h>
#include <Psapi.h>

#define SDF_HOOK_FAILED 0

class SDF {
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

    typedef std::function<void(Event event)> EventCallback;
    typedef std::function<void(AssetType type, const std::string& id, RC::Unreal::UObject* asset)> CreateAssetCallback;

protected:
    static inline std::unordered_map<uint64_t, EventCallback> eventCallbacks{};
    static inline std::unordered_map<uint64_t, CreateAssetCallback> createAssetCallbacks{};

    typedef SDF*(__cdecl *GetterFunc)();
    static inline SDF* instance = nullptr;

    typedef void(__cdecl *EventCallbackC)(uint64_t hookId, Event event);
    typedef void(__cdecl *CreateAssetCallbackC)(uint64_t hookId, AssetType type, const std::string& id, RC::Unreal::UObject* asset);

    virtual ~SDF() = default;

    virtual uint64_t HookEventInternal(EventCallbackC callback) = 0;
    virtual uint64_t HookCreateAssetInternal(CreateAssetCallbackC callback) = 0;
    virtual void UnhookInternal(uint64_t hookId) = 0;

    static SDF* Get() {
        if (instance == nullptr) {
            HMODULE mods[4096];
            DWORD cb;

            HANDLE currentProc = GetCurrentProcess();
            EnumProcessModules(currentProc, mods, sizeof(mods), &cb);

            for (unsigned i = 0; i < cb; i++) {
                wchar_t modPath[MAX_PATH];
                GetModuleFileNameExW(currentProc, mods[i], modPath, MAX_PATH);

                if (!std::wstring(modPath).ends_with(L"Mods\\SDF\\dlls\\main.dll"))
                    continue;

                const auto func = reinterpret_cast<GetterFunc>(GetProcAddress(mods[i], "sdf_get_sdf"));
                if (func == nullptr)
                    return nullptr;

                instance = func();
            }
        }
        return instance;
    }

    static void InvokeEvent(const uint64_t hookId, const Event event) {
        eventCallbacks[hookId](event);
    }

    static void InvokeCreateAsset(const uint64_t hookId, const AssetType type, const std::string& id, RC::Unreal::UObject* asset) {
        createAssetCallbacks[hookId](type, id, asset);
    }

public:
    static uint64_t HookEvent(EventCallback callback) {
        if (Get() == nullptr)
            return 0;
        const auto hookId = Get()->HookEventInternal(InvokeEvent);
        eventCallbacks[hookId] = std::move(callback);
        return hookId;
    }

    static uint64_t HookCreateAsset(CreateAssetCallback callback) {
        if (Get() == nullptr)
            return 0;
        const auto hookId = Get()->HookCreateAssetInternal(InvokeCreateAsset);
        createAssetCallbacks[hookId] = std::move(callback);
        return hookId;
    }

    static void Unhook(const uint64_t hookId) {
        Get()->UnhookInternal(hookId);
        eventCallbacks.erase(hookId);
        createAssetCallbacks.erase(hookId);
    }
};
