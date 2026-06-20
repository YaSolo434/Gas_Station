// YaSolo
#include "UserInterface/Inventory/Hotbar/HotbarPanel.h"

#include "Characters/PlayerCharacter.h"
#include "Components/HorizontalBox.h"
#include "Components/InventoryComponent.h"
#include "UserInterface/Inventory/Hotbar/HotbarItemSlot.h"

void UHotbarPanel::NativeConstruct()
{
	Super::NativeConstruct();

	if (const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerPawn()))
	{
		Inventory = PlayerCharacter->GetInventory();

		//subscribe to channels
		Inventory->OnInventoryUpdated.AddDynamic(this, &UHotbarPanel::RefreshHotbar);
		Inventory->OnSelectedSlotChanged.AddDynamic(this, &UHotbarPanel::UpdateSelectedSlot);

		for (int32 i = 0; i < Inventory->GetSlotsCapacity(); i++)
		{
			SingleSlotWidget = CreateWidget<UHotbarItemSlot>(this, HotbarItemSlotClass);
			HorizontalBox->AddChildToHorizontalBox(SingleSlotWidget);
			SlotWidgets.Add(SingleSlotWidget);
		}

		// RefreshHotbar();
		UpdateSelectedSlot(0);
	}
}

void UHotbarPanel::RefreshHotbar(const int32 NewSlot)
{
	for (int32 i = 0; i < SlotWidgets.Num(); i++)
	{
		SlotWidgets[i]->SetItem(Inventory->GetInventoryContents()[i]);
	}
}

void UHotbarPanel::UpdateSelectedSlot(const int32 NewSlot)
{
	for (int32 i = 0; i < SlotWidgets.Num(); i++)
	{
		SlotWidgets[i]->SetSelected(i == NewSlot);
	}
}
