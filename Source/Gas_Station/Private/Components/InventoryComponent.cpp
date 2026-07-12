// YaSolo
#include "Components/InventoryComponent.h"
#include "Items/ItemBase.h"
#include "UserInterface/BurgerHUD.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	InventoryContents.Init(nullptr, InventorySlotsCapacity);
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	HUD = Cast<ABurgerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* ItemIn)
{
	if (GetOwner())
	{
		if (InventoryContents[SelectedSlot] == nullptr)
		{
			AddNewItem(ItemIn, SelectedSlot);
			return FItemAddResult::AddedItem(FText::Format(
				FText::FromString("Successfully added {0} to the selected slot"),
				ItemIn->DescriptiveText.DisplayName));
		}

		for (int32 Slot = 0; Slot < InventoryContents.Num(); Slot++)
		{
			if (InventoryContents[Slot] == nullptr)
			{
				AddNewItem(ItemIn, Slot);
				return FItemAddResult::AddedItem(FText::Format(
					FText::FromString("Successfully added {0} to the hotbar"),
					ItemIn->DescriptiveText.DisplayName));
			}
		}

		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. Inventory is full"),
			ItemIn->DescriptiveText.DisplayName));
	}

	check(false)
	return FItemAddResult::AddedNone(FText::FromString("Try add item fallthrough error. GetOwner() somehow failed."));
}

void UInventoryComponent::AddNewItem(UItemBase* Item, int32 SlotIndex)
{
	UItemBase* NewItem;

	if (Item->bIsCopy || Item->bIsPickup)
	{
		// if the item is already a copy, or is a world pickup
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		// used when dragging to/from another inventory
		NewItem = Item->CreateItemCopy();
	}

	NewItem->OwningInventory = this;

	InventoryContents[SlotIndex] = NewItem;

	HUD->UpdateHighlightWidget(InventoryContents[SlotIndex]);

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_HighlightWidget,
		HUD,
		&ABurgerHUD::HideHighlightWidget,
		HighlightWidgetDurationTime,
		false
	);
	OnInventoryUpdated.Broadcast(SlotIndex);
}

UItemBase* UInventoryComponent::RemoveSelectedItem()
{
	UItemBase* ItemToRemove = InventoryContents[SelectedSlot];
	InventoryContents[SelectedSlot] = nullptr;
	OnInventoryUpdated.Broadcast(SelectedSlot);

	return ItemToRemove;
}

void UInventoryComponent::ScrollHotbar(const int32 Direction)
{
	SelectSlot(SelectedSlot - Direction);
}

void UInventoryComponent::SelectSlot(const int32 Index)
{
	const int32 New = WrapIndex(Index);
	if (New == SelectedSlot)
	{
		return;
	}

	// clears the old timer
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_HighlightWidget);

	SelectedSlot = New;
	OnSelectedSlotChanged.Broadcast(SelectedSlot);

	HUD->HideHighlightWidget();

	if (InventoryContents[SelectedSlot])
	{
		HUD->UpdateHighlightWidget(InventoryContents[SelectedSlot]);

		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_HighlightWidget,
			HUD,
			&ABurgerHUD::HideHighlightWidget,
			HighlightWidgetDurationTime,
			false
		);
	}
}

int32 UInventoryComponent::WrapIndex(const int32 Index) const
{
	int32 Wrapped = Index % InventorySlotsCapacity;

	if (Wrapped < 0)
	{
		Wrapped += InventorySlotsCapacity;
	}

	return Wrapped;
}
