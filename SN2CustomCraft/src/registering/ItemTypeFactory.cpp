//
// Created by Limo on 25/05/2026.
//

#include "ItemTypeFactory.hpp"

#include "RecipeFactory.hpp"
#include "UObjectGlobals.hpp"
#include "SDK/AssetRegistry_classes.hpp"
#include "util/Finders.hpp"
#include "util/Log.hpp"

using namespace SDK;
using namespace RC;
using namespace Unreal;

using EF = SDK::EObjectFlags;

std::vector<UUWEItemType*> ItemTypeFactory::registeredItemTypes;

ItemTypeFactory::ItemTypeFactory(std::string itemId, std::string itemName, std::string itemDescription)
    : itemId(std::move(itemId)), itemName(std::move(itemName)), itemDescription(std::move(itemDescription)) {

    const auto defaultTex = reinterpret_cast<UTexture2D*>(UObjectGlobals::FindObject(L"Texture2D", L"T_DefaultImage"));
    setIcon(defaultTex);
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
    itemTexture = static_cast<TSoftObjectPtr<UTexture2D>>(UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(icon));
    return true;
}

UUWEItemType* ItemTypeFactory::registerItemType() const {
    const auto base = Finders::searchItem("Titanium");
    if (base == nullptr)
        return nullptr;

    const auto itemType = static_cast<UUWEItemType*>(UGameplayStatics::SpawnObject(UUWEItemType::StaticClass(), base->Outer));
    if (itemType == nullptr)
        return nullptr;

    itemType->Name = UKismetStringLibrary::Conv_StringToName(UtfN::StringToWString(std::format("DA_{}_ItemType", itemId)).c_str());
    itemType->Flags = EF::MarkAsRootSet | EF::Public | EF::Standalone | EF::Transactional | EF::WasLoaded | EF::LoadCompleted;

    itemType->Name_0 = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(itemName).c_str());
    itemType->ItemDescription = UKismetTextLibrary::Conv_StringToText(UtfN::StringToWString(itemDescription).c_str());
    itemType->Thumbnail = itemTexture;

    itemType->FabricationPreviewMaterialInstance = base->FabricationPreviewMaterialInstance;
    itemType->FabricationPreviewMeshData = base->FabricationPreviewMeshData;

    itemType->ActorClass = base->ActorClass;

    registeredItemTypes.push_back(itemType);

    const auto assetData = UAssetRegistryHelpers::CreateAssetData(itemType, true);
    if (UAssetRegistryHelpers::GetAsset(assetData)) {
        Log::Verbose("Yippie");
    } else {
        Log::Verbose("Not yippie");
    }

    Log::Verbose("Item type registered: {}", itemId);
    return itemType;
}

