// YaSolo

#pragma once

#include <ThirdParty/hlslcc/hlslcc/src/hlslcc_lib/ir.h>

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Data/ItemDataStruct.h"

#include "ItemBase.generated.h"

class UInventoryComponent;
class APlayerCharacter;

UCLASS()
class GAS_STATION_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
	//=================================================================================================
	// PROPERTIES & VARIABLES
	//=================================================================================================
	UPROPERTY()
	UInventoryComponent* OwningInventory;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FItemAssetData AssetData;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	EItemType ItemType;
	
	UPROPERTY(EditAnywhere, Category = "ItemData")
	EFoodType FoodType;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FItemDescriptiveTextData DescriptiveText;

	bool bIsCopy;
	bool bIsPickup;

	//=================================================================================================
	// FUNCTIONS
	//=================================================================================================
	UItemBase();

	void ResetItemFlags();

	UFUNCTION(Category = "Item")
	UItemBase* CreateItemCopy() const;

	UFUNCTION(Category = "Item")
	virtual void Use(APlayerCharacter* PlayerCharacter);

protected:
	bool operator==(const FName& OtherID) const
	{
		return this->ID == OtherID;
	}
};
