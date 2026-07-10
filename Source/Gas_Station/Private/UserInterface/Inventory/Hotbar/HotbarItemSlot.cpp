// YaSolo
#include "UserInterface/Inventory/Hotbar/HotbarItemSlot.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Items/ItemBase.h"

void UHotbarItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ItemBorderTexture)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemBorderTexture is null"));
	}

	// ItemBorder->SetBrushFromTexture(ItemBorderTexture);
	// ItemBorder->SetContentColorAndOpacity(FLinearColor::White);

	ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
}

void UHotbarItemSlot::SetItem(const UItemBase* Item) const
{
	if (Item && Item->AssetData.Icon)
	{
		// Debug check
		UE_LOG(LogTemp, Warning, TEXT("Icon: %s, SRGB: %d, Width: %d, Height: %d"),
		       *Item->AssetData.Icon->GetName(),
		       Item->AssetData.Icon->SRGB,
		       Item->AssetData.Icon->GetSizeX(),
		       Item->AssetData.Icon->GetSizeY());

		ItemIcon->SetBrushFromTexture(Item->AssetData.Icon);
		ItemIcon->SetColorAndOpacity(FLinearColor::White);
		ItemIcon->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UHotbarItemSlot::SetSelected(const bool bIsSelected) const
{
	UTexture2D* Texture = bIsSelected ? SelectedItemTexture : ItemBorderTexture;

	ItemBorder->SetBrushFromTexture(Texture);
}
