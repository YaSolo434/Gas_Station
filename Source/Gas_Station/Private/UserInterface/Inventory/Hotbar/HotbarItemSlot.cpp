// YaSolo
#include "UserInterface/Inventory/Hotbar/HotbarItemSlot.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Items/ItemBase.h"

void UHotbarItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	ItemBorder->SetBrushColor(FLinearColor(FLinearColor::Red));
	ItemIcon->SetVisibility(ESlateVisibility::Hidden);
}

void UHotbarItemSlot::SetItem(UItemBase* Item)
{
	ItemReference = Item;

	if (Item)
	{
		ItemIcon->SetBrushFromTexture(Item->AssetData.Icon);
		ItemIcon->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ItemIcon->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHotbarItemSlot::SetSelected(const bool bIsSelected) const
{
	const FLinearColor Color = bIsSelected
		                           ? FLinearColor(1.f, 1.f, 1.f, 1.f) // white when selected
		                           : FLinearColor(FLinearColor::Red); // gray when not

	ItemBorder->SetBrushColor(Color);
}
