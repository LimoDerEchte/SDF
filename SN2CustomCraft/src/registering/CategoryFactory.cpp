//
// Created by Limo on 23/05/2026.
//

#include "registering/CategoryFactory.hpp"

#include "registering/RecipeFactory.hpp"
#include "UObjectGlobals.hpp"
#include "util/Log.hpp"
#include "UObject.hpp"
#include "util/Finders.hpp"
#include "util/RegistryHelper.hpp"

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

CategoryFactory::CategoryFactory(std::string categoryId, const bool modifyMode)
    : categoryId(std::move(categoryId)), categoryName(std::move("Empty")), categoryDescription(std::move("Empty")), crafterType(ECrafterType::Undefined), modifyMode(modifyMode) {
    if (!modifyMode) {
        const auto defaultTex = reinterpret_cast<UTexture2D*>(UObjectGlobals::FindObject(L"Texture2D", L"T_DefaultImage"));
        setIcon(defaultTex);
    }
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
    categoryTexture = static_cast<TSoftObjectPtr<UTexture2D>>(UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(icon));
    return true;
}

void CategoryFactory::setIcon(const TSoftObjectPtr<UTexture2D> &icon) {
    categoryTextureModified = true;
    categoryTexture = icon;
}

UUWECraftingRecipeCategory *CategoryFactory::registerCategory() const {
    const auto base = reinterpret_cast<UUWECraftingRecipeCategory*>(UObjectGlobals::FindObject(L"UWECraftingRecipeCategory", L"DA_Processor"));
    if (base == nullptr)
        return nullptr;

    const auto recipeCategory = modifyMode ? Finders::searchRecipeCategory(categoryId) : static_cast<UUWECraftingRecipeCategory*>(UGameplayStatics::SpawnObject(UUWECraftingRecipeCategory::StaticClass(), base->Outer));
    if (recipeCategory == nullptr)
        return nullptr;

    if (!modifyMode) {
        recipeCategory->Name = UKismetStringLibrary::Conv_StringToName(UtfN::StringToWString(std::format("DA_{}", categoryId)).c_str());
        recipeCategory->Flags = EF::MarkAsRootSet | EF::Public | EF::Standalone | EF::Transactional | EF::WasLoaded | EF::LoadCompleted;
    }
    if (orderingIndexModify)
        recipeCategory->OrderingIndex = orderingIndex;
    if (showWhenEmptyModify)
        recipeCategory->bShowWhenEmpty = showWhenEmpty;

    if (!modifyMode || categoryName != "Empty")
        recipeCategory->Name_0 = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(categoryName).c_str());
    if (!modifyMode || categoryDescription != "Empty")
        recipeCategory->Description = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(categoryDescription).c_str());
    if (categoryTextureModified)
        recipeCategory->Thumbnail = categoryTexture;
    if (modifyCrafterType)
        recipeCategory->CraftedBy = crafterType;

    if (categoryParent != nullptr)
        recipeCategory->ParentCategory = static_cast<TSoftObjectPtr<UUWECraftingRecipeCategory>>(UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(categoryParent));

#ifdef DEVELOPMENT
    RegistryHelper::AddToRegistry(recipeCategory, "CraftingRecipeCategory");
    if (rootCategory && !modifyMode) {

        Log::Verbose("Registering root category to UWECrafterComponent");
        std::string searchString = "Crafting/BP_Fabricator.Default__BP_Fabricator_C:Crafter";
        auto comp = Finders::searchComponent(searchString);
        if (comp != nullptr) {
            const auto categories = reinterpret_cast<Unreal::TArray<TSoftObjectPtr<UUWECraftingRecipeCategory>>*>(&comp->AllowedRecipeCategories);
            categories->Emplace(UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(recipeCategory));
        } else
            Log::Warning("Finders::searchComponent failed: {}", searchString);
    }
#endif

    registeredCategories.push_back(recipeCategory);
    Log::Verbose("Recipe category {}: {}", modifyMode ? "modified" : "registered", categoryId);
    return recipeCategory;
}
