//
// Created by Limo on 27/06/2026.
//

#include "SDF.hpp"

uint64_t SDF_Impl::incrementor = 0;
SDF_Impl *SDF_Impl::internalInstance = new SDF_Impl();

std::unordered_map<uint64_t, SDF::EventCallbackC> SDF_Impl::eventHooks;
std::unordered_map<uint64_t, SDF::CreateAssetCallbackC> SDF_Impl::createAssetHooks;

uint64_t SDF_Impl::HookEventInternal(EventCallbackC callback) {
    const uint64_t id = incrementor++;
    eventHooks.emplace(id, std::move(callback));
    return id;
}

uint64_t SDF_Impl::HookCreateAssetInternal(CreateAssetCallbackC callback) {
    const uint64_t id = incrementor++;
    createAssetHooks.emplace(id, std::move(callback));
    return id;
}

void SDF_Impl::UnhookInternal(const uint64_t hookId) {
    eventHooks.erase(hookId);
    createAssetHooks.erase(hookId);
}

SDF_Impl *SDF_Impl::InternalInstance() {
    return internalInstance;
}

void SDF_Impl::TriggerEvent(const Event event) {
    for (const auto [hookId, callback]: eventHooks)
        callback(hookId, event);
}

void SDF_Impl::TriggerCreateAsset(const AssetType type, const std::string &id, RC::Unreal::UObject *asset) {
    for (const auto [hookId, callback]: createAssetHooks)
        callback(hookId, type, id, asset);
}

extern "C" __declspec(dllexport) inline SDF_Impl* sdf_get_sdf() {
    return SDF_Impl::InternalInstance();
}
