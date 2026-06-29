//
// Created by Limo on 27/06/2026.
//

#pragma once

#include "SDFRecipe.hpp"
#include "SDFCategory.hpp"

#include <UObject.hpp>

#include <functional>
#include <string>

#include <Windows.h>
#include <Psapi.h>

class SDF {
public:
    static constexpr int CurrentAPIVersion = 1;

    enum AssetType {
        Recipe,
        Category,
        DatabankEntry,
    };

    enum Event {
        PreTraverse,
        PostTraverse,
        PreCategory,
        PostCategory,
        PreRecipe,
        PostRecipe,
        PreDatabankEntry,
        PostDatabankEntry,
    };

    typedef std::function<void(Event event)> EventCallback;
    typedef std::function<void(AssetType type, const std::string& id, RC::Unreal::UObject* asset)> CreateAssetCallback;

protected:
    static inline std::unordered_map<int64_t, EventCallback> eventCallbacks{};
    static inline std::unordered_map<int64_t, CreateAssetCallback> createAssetCallbacks{};

    typedef SDF*(__cdecl *GetterFunc)();
    static inline SDF* instance = nullptr;

    typedef void(__cdecl *EventCallbackC)(int64_t hookId, Event event);
    typedef void(__cdecl *CreateAssetCallbackC)(int64_t hookId, AssetType type, const std::string& id, RC::Unreal::UObject* asset);

    virtual ~SDF() = default;

    virtual uint64_t GetCurrentVersion() = 0;
    virtual uint64_t GetLowestSupportedVersion() = 0;

    virtual int64_t HookEventInternal(EventCallbackC callback) = 0;
    virtual int64_t HookCreateAssetInternal(CreateAssetCallbackC callback) = 0;
    virtual void UnhookInternal(int64_t hookId) = 0;

    virtual std::unique_ptr<SDFRecipe> CreateRecipeFactory(const std::string &id, bool modifyMode) = 0;
    virtual void DeleteCraftingRecipe(const std::string &id) = 0;

    virtual std::unique_ptr<SDFCategory> CreateCategoryFactory(const std::string &id, bool modifyMode) = 0;
    virtual void DeleteCraftingRecipeCategory(const std::string &id) = 0;

    static SDF* Get() {
        if (instance == nullptr) {
            HMODULE mods[4096];
            DWORD cb;

            HANDLE currentProc = GetCurrentProcess();
            EnumProcessModules(currentProc, mods, sizeof(mods), &cb);

            for (unsigned i = 0; i < cb; i++) {
                const auto func = reinterpret_cast<GetterFunc>(GetProcAddress(mods[i], "sdf_get_sdf"));
                if (func == nullptr)
                    continue;

                instance = func();
                break;
            }

            if (!instance)
                throw std::runtime_error("Could not acquire SDF C++ API handle");

            if (uint64_t sdfAPIVersion = instance->GetCurrentVersion(); CurrentAPIVersion > sdfAPIVersion)
                throw std::runtime_error(std::format("SDF API version is too old ({} > {})", CurrentAPIVersion, sdfAPIVersion));

            if (uint64_t lowestAPIVersion = instance->GetLowestSupportedVersion(); CurrentAPIVersion < lowestAPIVersion)
                throw std::runtime_error(std::format("SDF API does not support this API version anymore ({} < {})", CurrentAPIVersion, lowestAPIVersion));
        }
        return instance;
    }

    static void InvokeEvent(const int64_t hookId, const Event event) {
        eventCallbacks[hookId](event);
    }

    static void InvokeCreateAsset(const int64_t hookId, const AssetType type, const std::string& id, RC::Unreal::UObject* asset) {
        createAssetCallbacks[hookId](type, id, asset);
    }

public:
    /**
     * Registers a callback to be invoked every time an SDF event is fired
     * @param callback This callback is invoked on every event
     * @return Returns a unique hook id used in SDF::Unhook
     */
    static int64_t HookEvent(EventCallback callback) {
        const auto hookId = Get()->HookEventInternal(InvokeEvent);
        eventCallbacks[hookId] = std::move(callback);
        return hookId;
    }

    /**
     * Registers a callback to be invoked every time SDF has registered or modified an asset
     * @param callback This callback is invoked on every event
     * @return Returns a unique hook id used in SDF::Unhook
     */
    static int64_t HookCreateAsset(CreateAssetCallback callback) {
        const auto hookId = Get()->HookCreateAssetInternal(InvokeCreateAsset);
        createAssetCallbacks[hookId] = std::move(callback);
        return hookId;
    }

    /**
     * Unregisters a previously registered hook and disposes of the callback
     * @param hookId The previously obtained hook id
     */
    static void Unhook(const int64_t hookId) {
        Get()->UnhookInternal(hookId);
        eventCallbacks.erase(hookId);
        createAssetCallbacks.erase(hookId);
    }

    /**
     * Creates a builder that can be used for creating a new crafting recipe
     * @param id The id of the new crafting recipe
     * @return Returns a builder to construct the new recipe
     */
    static std::unique_ptr<SDFRecipe> CreateRecipe(const std::string &id) {
        return Get()->CreateRecipeFactory(id, false);
    }

    /**
     * Creates a builder that can be used for modifying an existing crafting recipe
     * @param id The id of the existing crafting recipe according to the recipe notation: https://sn2-sdf.dev/generic/notations/#crafting-recipes
     * @return Returns a builder to modify the existing recipe
     */
    static std::unique_ptr<SDFRecipe> ModifyRecipe(const std::string &id) {
        return Get()->CreateRecipeFactory(id, true);
    }

    /**
     * Moves an existing crafting recipe to an invalid category, effectively deleting it in the process
     * @param id The id of the existing crafting recipe according to the recipe notation: https://sn2-sdf.dev/generic/notations/#crafting-recipes
     */
    static void DeleteRecipe(const std::string &id) {
        Get()->DeleteCraftingRecipe(id);
    }

    /**
     * Creates a builder that can be used for creating a new crafting recipe category
     * @param id The id of the new crafting recipe category
     * @return Returns a builder to construct the new category
     */
    static std::unique_ptr<SDFCategory> CreateRecipeCategory(const std::string &id) {
        return Get()->CreateCategoryFactory(id, false);
    }

    /**
     * Creates a builder that can be used for modifying an existing crafting recipe category
     * @param id The id of the existing crafting recipe category according to the category notation: https://sn2-sdf.dev/generic/notations/#crafting-recipe-categories
     * @return Returns a builder to modify the existing category
     */
    static std::unique_ptr<SDFCategory> ModifyRecipeCategory(const std::string &id) {
        return Get()->CreateCategoryFactory(id, true);
    }

    /**
     * Moves an existing crafting recipe category to an invalid category, effectively deleting it in the process
     * @param id The id of the existing crafting recipe category according to the category notation: https://sn2-sdf.dev/generic/notations/#crafting-recipe-categories
     */
    static void DeleteRecipeCategory(const std::string &id) {
        Get()->DeleteCraftingRecipeCategory(id);
    }
};
