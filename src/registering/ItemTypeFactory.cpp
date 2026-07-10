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

bool ItemTypeFactory::setActor(const std::string &actorPath) {
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
}

bool ItemTypeFactory::setIconFromItem(const std::string &itemId) {
    return setIconFromItem(Finders::searchItem(itemId));
}

bool ItemTypeFactory::setIconFromItem(UUWEItemType *item) {
    if (item == nullptr)
        return false;
    itemTexture = item->GetThumbnail();
    return true;
}

bool ItemTypeFactory::setIcon(UTexture2D *icon) {
    if (icon == nullptr)
        return false;
    itemTextureModified = true;
    itemTexture = icon;
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

    itemType->SetName_0(FText(UtfN::StringToWString(itemName).c_str()));
    itemType->SetItemDescription(FText(UtfN::StringToWString(itemDescription).c_str()));
    itemType->SetThumbnail(itemTexture);

    itemType->SetFabricationPreviewMaterialInstance(base->GetFabricationPreviewMaterialInstance());
    itemType->SetFabricationPreviewMeshData(*base->GetFabricationPreviewMeshData());

    if (actorClassModified)
        itemType->SetActorClass(actorClass);
    else if (!modifyMode)
        itemType->SetActorClass(*base->GetActorClass());

    itemType->SetTypeTag(FGameplayTag(UtfN::StringToWString(std::format("ItemType.Custom.{}", itemId))));
    itemType->SetGameplayTags(*base->GetGameplayTags());

    // TODO: Implement Tunable Data

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

