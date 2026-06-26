//
// Created by Limo on 25/05/2026.
//

#include "registering/ItemTypeFactory.hpp"

#include "UObject.hpp"
#include "registering/RecipeFactory.hpp"
#include "UObjectGlobals.hpp"
#include "SDK/AssetRegistry_classes.hpp"
#include "util/Finders.hpp"
#include "util/Log.hpp"
#include "util/RegistryHelper.hpp"

using namespace SDK;
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

bool ItemTypeFactory::setActor(const std::string &actorPath) {
    const auto path = UKismetSystemLibrary::MakeSoftClassPath(UtfN::StringToWString(actorPath).c_str());
    setActor(UKismetSystemLibrary::Conv_SoftClassPathToSoftClassRef(path));
    return true;
}

bool ItemTypeFactory::setActor(SDK::UClass *actorClass) {
    if (actorClass == nullptr)
        return false;
    actorClassModified = true;
    this->actorClass = UKismetSystemLibrary::Conv_ClassToSoftClassReference(actorClass);
    return true;
}

void ItemTypeFactory::setActor(const TSoftClassPtr<SDK::UClass> &actorClass) {
    actorClassModified = true;
    this->actorClass = actorClass;
}

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
    itemTexture = static_cast<TSoftObjectPtr<UTexture2D>>(UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(icon));
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

    itemType->Name_0 = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(itemName).c_str());
    itemType->ItemDescription = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(itemDescription).c_str());
    itemType->Thumbnail = itemTexture;

    itemType->FabricationPreviewMaterialInstance = base->FabricationPreviewMaterialInstance;
    itemType->FabricationPreviewMeshData = base->FabricationPreviewMeshData;

    if (actorClassModified)
        itemType->ActorClass = actorClass;
    else if (!modifyMode)
        itemType->ActorClass = base->ActorClass;

    itemType->TypeTag = FGameplayTag(UKismetStringLibrary::Conv_StringToName(UtfN::StringToWString(std::format("ItemType.Custom.{}", itemId)).c_str()));
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

