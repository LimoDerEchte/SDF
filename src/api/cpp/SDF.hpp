//
// Created by Limo on 27/06/2026.
//

#pragma once

#include "SDF/SDF.hpp"

class SDF_Impl final : public SDF {
    static int64_t incrementor;
    static SDF_Impl* internalInstance;

    static std::unordered_map<int64_t, EventCallbackC> eventHooks;
    static std::unordered_map<int64_t, CreateAssetCallbackC> createAssetHooks;

protected:
    int64_t HookEventInternal(EventCallbackC callback) override;
    int64_t HookCreateAssetInternal(CreateAssetCallbackC callback) override;
    void UnhookInternal(int64_t hookId) override;

    std::unique_ptr<SDFRecipe> CreateRecipeFactory(const std::string &id, bool modifyMode) override;
    void DeleteCraftingRecipe(const std::string &id) override;

public:
    static SDF_Impl* InternalInstance();
    static void TriggerEvent(Event event);
    static void TriggerCreateAsset(AssetType type, const std::string& id, RC::Unreal::UObject* asset);
};
