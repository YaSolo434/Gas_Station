// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HotbarPanel.generated.h"

class UInventoryComponent;
class UHorizontalBox;
class UHotbarItemSlot;
/**
 * 
 */
UCLASS()
class GAS_STATION_API UHotbarPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Hotbar")
	TSubclassOf<UHotbarItemSlot> HotbarItemSlotClass;

protected:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HorizontalBox;

	UPROPERTY()
	TArray<UHotbarItemSlot*> SlotWidgets;

	UPROPERTY()
	UHotbarItemSlot* SingleSlotWidget;

	UPROPERTY()
	UInventoryComponent* Inventory;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void RefreshHotbar(const int32 NewSlot);

	UFUNCTION()
	void UpdateSelectedSlot(const int32 NewSlot);
};
