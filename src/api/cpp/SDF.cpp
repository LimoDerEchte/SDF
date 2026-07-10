//
// Created by Limo on 27/06/2026.
//

#include "SDF.hpp"

#include "FProperty.hpp"
#include "SDFCategory.hpp"
#include "SDFRecipe.hpp"
#include "UKismetSystemLibrary.hpp"
#include "util/Finders.hpp"
#include "registering/RecipeFactory.hpp"

int64_t SDF_Impl::incrementor = 0;
SDF_Impl *SDF_Impl::internalInstance = new SDF_Impl();

std::unordered_map<int64_t, SDF::EventCallbackC> SDF_Impl::eventHooks;
std::unordered_map<int64_t, SDF::CreateAssetCallbackC> SDF_Impl::createAssetHooks;

uint64_t SDF_Impl::GetCurrentVersion() {
    return CurrentAPIVersion;
}

uint64_t SDF_Impl::GetLowestSupportedVersion() {
    return LowestSupportedVersion;
}

int64_t SDF_Impl::HookEventInternal(EventCallbackC callback) {
    const int64_t id = incrementor++;
    eventHooks.emplace(id, callback);
    return id;
}

int64_t SDF_Impl::HookCreateAssetInternal(CreateAssetCallbackC callback) {
    const int64_t id = incrementor++;
    createAssetHooks.emplace(id, callback);
    return id;
}

void SDF_Impl::UnhookInternal(const int64_t hookId) {
    eventHooks.erase(hookId);
    createAssetHooks.erase(hookId);
}

std::unique_ptr<SDFRecipe> SDF_Impl::CreateRecipeFactory(const std::string &id, bool modifyMode) {
    return std::make_unique<SDFRecipe_Impl>(id, modifyMode);
}

void SDF_Impl::DeleteCraftingRecipe(const std::string &id) {
    RecipeFactory factory(id, true);
    factory.setCategory("Fabricator");
    const auto _ = factory.registerRecipe();
}

std::unique_ptr<SDFCategory> SDF_Impl::CreateCategoryFactory(const std::string &id, bool modifyMode) {
    return std::make_unique<SDFCategory_Impl>(id, modifyMode);
}

void SDF_Impl::DeleteCraftingRecipeCategory(const std::string &id) {
    const auto cat = Finders::searchRecipeCategory(id);
    cat->SetParentCategory(nullptr);
}

SDF_Impl *SDF_Impl::InternalInstance() {
    return internalInstance;
}

void SDF_Impl::TriggerEvent(const Event event) {
    for (const auto [hookId, callback]: eventHooks)
        callback(hookId, event);
}

void SDF_Impl::TriggerCreateAsset(const AssetType type, const std::string &id, UObject *asset) {
    for (const auto [hookId, callback]: createAssetHooks)
        callback(hookId, type, id, asset);
}

extern "C" {
    __declspec(dllexport) inline SDF* sdf_get_sdf() {
        return SDF_Impl::InternalInstance();
    }
}