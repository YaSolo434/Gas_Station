// YaSolo
#include "Items/ItemBase.h"

UItemBase::UItemBase() : bIsCopy(false), bIsPickup(false)
{
}

void UItemBase::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;
}

UItemBase* UItemBase::CreateItemCopy() const
{
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass());

	ItemCopy->ID = this->ID;
	ItemCopy->AssetData = this->AssetData;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->DescriptiveText = this->DescriptiveText;
	ItemCopy->bIsCopy = true;

	return ItemCopy;
}

void UItemBase::Use(APlayerCharacter* PlayerCharacter)
{
}
