//
// Created by Limo on 27/06/2026.
//

#include "SDF.hpp"

uint64_t SDF_Impl::incrementor = 0;

std::unordered_map<uint64_t, SDF::EventCallback> SDF_Impl::eventHooks;
std::unordered_map<uint64_t, SDF::CreateAssetCallback> SDF_Impl::createAssetHooks;

void SDF_Impl::TriggerEvent(const Event event) {
    for (const auto val: eventHooks | std::views::values)
        val(event);
}

void SDF_Impl::TriggerCreateAsset(const AssetType type, const std::string &id, RC::Unreal::UObject *asset) {
    for (const auto val : createAssetHooks | std::views::values)
        val(type, id, asset);
}

uint64_t SDF_Impl::HookEvent(EventCallback callback) {
    const uint64_t id = incrementor++;
    eventHooks.emplace(id, std::move(callback));
    return id;
}

uint64_t SDF_Impl::HookCreateAsset(CreateAssetCallback callback) {
    const uint64_t id = incrementor++;
    createAssetHooks.emplace(id, std::move(callback));
    return id;
}

void SDF_Impl::Unhook(const uint64_t hookId) {
    eventHooks.erase(hookId);
    createAssetHooks.erase(hookId);
}

const auto instance = new SDF_Impl();
extern "C" __declspec(dllexport) inline SDF_Impl* sdf_get_sdf() {
    return instance;
}
