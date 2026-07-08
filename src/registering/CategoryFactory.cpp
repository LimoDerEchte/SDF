//
// Created by Limo on 23/05/2026.
//

#include "registering/CategoryFactory.hpp"

#include "UKismetSystemLibrary.hpp"
#include "registering/RecipeFactory.hpp"
#include "UObjectGlobals.hpp"
#include "util/Log.hpp"
#include "UObject.hpp"
#include "api/cpp/SDF.hpp"
#include "util/Finders.hpp"
#include "util/RegistryHelper.hpp"

using namespace SDK;
using namespace RC;
using namespace Unreal;

CategoryFactory::CategoryFactory(std::string categoryId, const bool modifyMode)
    : categoryId(std::move(categoryId)), categoryName(std::move("Empty")), categoryDescription(std::move("Empty")), crafterType(ECrafterType::Undefined), modifyMode(modifyMode) {
    if (!modifyMode)
        setIcon(Finders::searchTexture("T_DefaultImage"));
}

void CategoryFactory::setName(const std::string &categoryName) {
    this->categoryName = categoryName;
}

void CategoryFactory::setDescription(const std::string &categoryDescription) {
    this->categoryDescription = categoryDescription;
}

void CategoryFactory::setCrafterType(const ECrafterType crafterType) {
    this->crafterType = crafterType;
    modifyCrafterType = true;

    if (modifyMode)
        return;

    switch (crafterType) {
        case ECrafterType::Fabricator:
            setParent("Fabricator");
            rootCategory = true;
            break;
        case ECrafterType::Refinery:
            setParent("Processor");
            break;
        case ECrafterType::ModificationStation:
            setParent("ModificationStation");
            rootCategory = true;
            break;
        case ECrafterType::VehicleFabricator:
            setParent("VehicleBay_Category");
            rootCategory = true;
            break;
        case ECrafterType::BuilderTool:
            setParent("BuilderToolCategory");
            break;
        default:
            Log::Warning("Crafter type used by {} is currently unsupported!", categoryId);
    }
}

void CategoryFactory::setOrderingIndex(const int orderingIndex) {
    this->orderingIndex = orderingIndex;
    orderingIndexModify = true;
}

void CategoryFactory::setShowWhenEmpty(const bool showWhenEmpty) {
    this->showWhenEmpty = showWhenEmpty;
    showWhenEmptyModify = true;
}

bool CategoryFactory::setParent(const std::string &categoryId) {
    return setParent(Finders::searchRecipeCategory(categoryId));
}

bool CategoryFactory::setParent(UUWECraftingRecipeCategory *category) {
    if (category == nullptr)
        return false;
    categoryParent = category;
    rootCategory = false;
    return true;
}

bool CategoryFactory::setIconFromItem(const std::string &itemId) {
    return setIconFromItem(Finders::searchItem(itemId));
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
    categoryTextureModified = true;
    *reinterpret_cast<Unreal::TSoftObjectPtr<>*>(&categoryTexture) = Unreal::UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(reinterpret_cast<Unreal::UObject*>(icon));
    return true;
}

void CategoryFactory::setIcon(const SDK::TSoftObjectPtr<UTexture2D> &icon) {
    categoryTextureModified = true;
    categoryTexture = icon;
}

UUWECraftingRecipeCategory *CategoryFactory::registerCategory() const {
    const auto base = Finders::searchFirstOf<UUWECraftingRecipeCategory>("UWECraftingRecipeCategory");
    if (base == nullptr)
        return nullptr;

    const auto category = modifyMode ? Finders::searchRecipeCategory(categoryId) : RegistryHelper::StaticConstructTemplate(base, std::format("DA_{}", categoryId));
    if (category == nullptr)
        return nullptr;

    if (orderingIndexModify)
        category->OrderingIndex = orderingIndex;
    if (showWhenEmptyModify)
        category->bShowWhenEmpty = showWhenEmpty;

    if (!modifyMode || categoryName != "Empty")
        *reinterpret_cast<Unreal::FText*>(&category->Name_0) = Unreal::FText(UtfN::StringToWString(categoryName).c_str());
    if (!modifyMode || categoryDescription != "Empty")
        *reinterpret_cast<Unreal::FText*>(&category->Description) = Unreal::FText(UtfN::StringToWString(categoryDescription).c_str());
    if (categoryTextureModified)
        category->Thumbnail = categoryTexture;
    if (modifyCrafterType)
        category->CraftedBy = crafterType;

    if (categoryParent != nullptr)
        *reinterpret_cast<Unreal::TSoftObjectPtr<>*>(&category->ParentCategory) = Unreal::UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(reinterpret_cast<Unreal::UObject*>(categoryParent));

    RegistryHelper::AddToRegistry(category, "UWECraftingRecipeCategory");

    Log::Verbose("Category {}: {}", modifyMode ? "modified" : "registered", categoryId);
    SDF_Impl::TriggerCreateAsset(SDF::Category, categoryId, reinterpret_cast<Unreal::UObject*>(category));
    return category;
}
