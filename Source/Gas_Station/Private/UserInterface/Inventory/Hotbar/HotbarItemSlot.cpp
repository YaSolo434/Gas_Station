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
		UE_LOG(LogTemp, Warning, TEXT("Itembordertexture is null"));
	}

	// ItemBorder->SetBrushFromTexture(ItemBorderTexture);
	// ItemBorder->SetContentColorAndOpacity(FLinearColor::White);

	ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
}

void UHotbarItemSlot::SetItem(const UItemBase* Item) const
{
	if (Item && Item->AssetData.Icon)
	{
		FSlateBrush NewBrush;
		NewBrush.SetResourceObject(Item->AssetData.Icon);
		NewBrush.TintColor = FSlateColor(FLinearColor::White);
		NewBrush.DrawAs = ESlateBrushDrawType::Image;
		NewBrush.ImageSize = FVector2D(Item->AssetData.Icon->GetSizeX(),
		                               Item->AssetData.Icon->GetSizeY());

		ItemIcon->SetBrush(NewBrush);
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

	// ItemBorder->SetBrushFromTexture(Texture);
}
