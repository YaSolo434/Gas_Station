// YaSolo
#include "Items/ItemBase.h"

UItemBase::UItemBase()
{
}

UItemBase* UItemBase::CreateItemCopy() const
{
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass());

	ItemCopy->ID = this->ID;
	ItemCopy->AssetData = this->AssetData;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->DescriptiveText = this->DescriptiveText;

	return ItemCopy;
}

void UItemBase::Use(APlayerCharacter* PlayerCharacter)
{
}
