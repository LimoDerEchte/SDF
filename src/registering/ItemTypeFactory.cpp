//
// Created by Limo on 25/05/2026.
//

#include "registering/ItemTypeFactory.hpp"

#include "FText.hpp"
#include "UKismetSystemLibrary.hpp"
#include "registering/RecipeFactory.hpp"
#include "util/Finders.hpp"
#include "util/Log.hpp"
#include "util/RegistryHelper.hpp"

using namespace RC;
using namespace Unreal;

ItemTypeFactory::ItemTypeFactory(std::string itemId, const bool modifyMode) : itemId(std::move(itemId)) {
    if (!modifyMode)
        setIcon(Finders::searchTexture("T_DefaultImage"));
}

void ItemTypeFactory::setName(const std::string &itemName) {
    this->itemName = itemName;
}

void ItemTypeFactory::setDescription(const std::string &itemDescription) {
    this->itemDescription = itemDescription;
}

/*bool ItemTypeFactory::setActor(const std::string &actorPath) {
    const auto path = UKismetSystemLibrary::MakeSoftClassPath(UtfN::StringToWString(actorPath).c_str());
    setActor(UKismetSystemLibrary::Conv_SoftClassPathToSoftClassRef(path));
    return true;
}

bool ItemTypeFactory::setActor(UClass *actorClass) {
    if (actorClass == nullptr)
        return false;
    actorClassModified = true;
    this->actorClass = UKismetSystemLibrary::Conv_ClassToSoftClassReference(actorClass);
    return true;
}

void ItemTypeFactory::setActor(const TSoftClassPtr<UClass> &actorClass) {
    actorClassModified = true;
    this->actorClass = actorClass;
}*/

bool ItemTypeFactory::setIconFromItem(const std::string &itemId) {
    return setIconFromItem(Finders::searchItem(itemId));
}

bool ItemTypeFactory::setIconFromItem(const UUWEItemType *item) {
    if (item == nullptr)
        return false;
    itemTexture = item->Thumbnail;
    return true;
}

bool ItemTypeFactory::setIcon(UTexture2D *icon) {
    if (icon == nullptr)
        return false;
    itemTextureModified = true;
    *reinterpret_cast<Unreal::TSoftObjectPtr<>*>(&itemTexture) = Unreal::UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(reinterpret_cast<Unreal::UObject*>(icon));
    return true;
}

void ItemTypeFactory::setIcon(const TSoftObjectPtr<UTexture2D> &icon) {
    itemTextureModified = true;
    itemTexture = icon;
}

UUWEItemType* ItemTypeFactory::registerItemType() const {
    const auto base = Finders::searchFirstOf<UUWEItemType>("UWEItemType");
    if (base == nullptr)
        return nullptr;

    const auto itemType = RegistryHelper::StaticConstructTemplate(base, std::format("DA_{}_ItemType", itemId));
    if (itemType == nullptr)
        return nullptr;

    *reinterpret_cast<Unreal::FText*>(&itemType->Name_0) = Unreal::FText(UtfN::StringToWString(itemName).c_str());
    *reinterpret_cast<Unreal::FText*>(&itemType->ItemDescription) = Unreal::FText(UtfN::StringToWString(itemDescription).c_str());
    itemType->Thumbnail = itemTexture;

    itemType->FabricationPreviewMaterialInstance = base->FabricationPreviewMaterialInstance;
    itemType->FabricationPreviewMeshData = base->FabricationPreviewMeshData;

    /*if (actorClassModified)
        itemType->ActorClass = actorClass;
    else if (!modifyMode)*/
        itemType->ActorClass = base->ActorClass;

    itemType->TypeTag = TempFinders::CreateTag(std::format("ItemType.Custom.{}", itemId));
    itemType->GameplayTags = base->GameplayTags;
    itemType->TunableData = base->TunableData;

    RegistryHelper::AddToRegistry(itemType, "UWEItemType");

    Log::Verbose("Item type registered: {}", itemId);
    return itemType;
}

UUWEItemType *ItemTypeFactory::registerItemType(const std::string &itemId, const std::string &path) {
    const auto base = Finders::searchItem("Titanium");
    if (base == nullptr)
        return nullptr;

    return base;
}

