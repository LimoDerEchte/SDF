//
// Created by Limo on 27/06/2026.
//

#pragma once

#include "SDF/SDF.hpp"

class SDF_Impl final : public SDF {
    static uint64_t incrementor;
    static SDF_Impl* internalInstance;

    static std::unordered_map<uint64_t, EventCallback> eventHooks;
    static std::unordered_map<uint64_t, CreateAssetCallback> createAssetHooks;

public:
    static SDF_Impl* InternalInstance();
    static void TriggerEvent(Event event);
    static void TriggerCreateAsset(AssetType type, const std::string& id, RC::Unreal::UObject* asset);

    uint64_t HookEvent(EventCallback callback) override;
    uint64_t HookCreateAsset(CreateAssetCallback callback) override;
    void Unhook(uint64_t hookId) override;
};
