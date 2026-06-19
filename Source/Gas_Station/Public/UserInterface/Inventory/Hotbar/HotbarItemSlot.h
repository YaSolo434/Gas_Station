// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HotbarItemSlot.generated.h"

class UImage;
class UBorder;
class UItemBase;
/**
 * 
 */
UCLASS()
class GAS_STATION_API UHotbarItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetItemReference(UItemBase* ItemIn) { ItemReference = ItemIn; }
	FORCEINLINE UItemBase* GetItemReference() const { return ItemReference; }

	void SetItem(UItemBase* Item);
	void SetSelected(bool bIsSelected) const;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere, Category="Hotbar Slot")
	UItemBase* ItemReference;
};
