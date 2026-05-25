//
// Created by Limo on 23/05/2026.
//

#include "CategoryFactory.hpp"

#include "RecipeFactory.hpp"
#include "UObjectGlobals.hpp"
#include "util/Log.hpp"
#include "UObject.hpp"

using namespace SDK;
using namespace RC;
using namespace Unreal;

using EF = SDK::EObjectFlags;

std::vector<UUWECraftingRecipeCategory*> CategoryFactory::registeredCategories;

void CategoryFactory::unregisterAllCategories() {
    for (const auto& registered_recipe : registeredCategories)
        reinterpret_cast<Unreal::UObject*>(registered_recipe)->BeginDestroy();
    registeredCategories.clear();
}

CategoryFactory::CategoryFactory(std::string categoryId, std::string categoryName, std::string categoryDescription, const ECrafterType crafterType)
    : categoryId(std::move(categoryId)), categoryName(std::move(categoryName)), categoryDescription(std::move(categoryDescription)), crafterType(crafterType) {

    const auto defaultTex = reinterpret_cast<UTexture2D*>(UObjectGlobals::FindObject(L"Texture2D", L"T_DefaultImage"));
    setIcon(defaultTex);

    switch (crafterType) {
        case ECrafterType::Fabricator:
            setParent("Fabricator");
            break;
        case ECrafterType::Refinery:
            setParent("Processor");
            break;
        case ECrafterType::ModificationStation:
            setParent("ModificationStation");
            break;
        case ECrafterType::VehicleFabricator:
            setParent("VehicleBay_Category");
            break;
        case ECrafterType::BuilderTool:
            setParent("BuilderToolCategory");
            break;
        default:
            Log::Warning("Crafter type used by {} is currently unsupported!", categoryId);
    }
}

bool CategoryFactory::setParent(const std::string &categoryId) {
    return setParent(RecipeFactory::searchRecipeCategory(categoryId));
}

bool CategoryFactory::setParent(UUWECraftingRecipeCategory *category) {
    if (category == nullptr)
        return false;
    categoryParent = category;
    return true;
}

bool CategoryFactory::setIconFromItem(const std::string &itemId) {
    return setIconFromItem(RecipeFactory::searchItem(itemId));
}

bool CategoryFactory::setIconFromItem(const UUWEItemType *item) {
    if (item == nullptr)
        return false;
    categoryTexture = item->Thumbnail;
    return true;
}

bool CategoryFactory::setIcon(UTexture2D *icon) {
    if (icon == nullptr)
        return false;
    categoryTexture = static_cast<TSoftObjectPtr<UTexture2D>>(UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(icon));
    return true;
}

UUWECraftingRecipeCategory *CategoryFactory::registerCategory() const {
    const auto base = reinterpret_cast<UUWECraftingRecipeCategory*>(UObjectGlobals::FindObject(L"UWECraftingRecipeCategory", L"DA_Processor"));
    if (base == nullptr)
        return nullptr;

    const auto recipe = static_cast<UUWECraftingRecipeCategory*>(UGameplayStatics::SpawnObject(UUWECraftingRecipeCategory::StaticClass(), base->Outer));
    if (recipe == nullptr)
        return nullptr;

    recipe->Name = UKismetStringLibrary::Conv_StringToName(UtfN::StringToWString(std::format("DA_{}", categoryId)).c_str());
    recipe->Flags = EF::MarkAsRootSet | EF::Public | EF::Standalone | EF::Transactional | EF::WasLoaded | EF::LoadCompleted;

    recipe->Name_0 = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(categoryName).c_str());
    recipe->Description = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(categoryDescription).c_str());
    recipe->Thumbnail = categoryTexture;
    recipe->CraftedBy = crafterType;

    if (categoryParent != nullptr)
        recipe->ParentCategory = static_cast<TSoftObjectPtr<UUWECraftingRecipeCategory>>(UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(categoryParent));

    registeredCategories.push_back(recipe);
    Log::Verbose("Recipe category registered: {}", categoryId);
    return recipe;
}
