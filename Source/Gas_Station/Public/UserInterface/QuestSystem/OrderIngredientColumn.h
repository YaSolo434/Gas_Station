// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/ItemDataStruct.h"
#include "OrderIngredientColumn.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class GAS_STATION_API UOrderIngredientColumn : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetIngredient(EFoodType FoodType, int32 Quantity);

protected:
	UPROPERTY(meta=(BindWidget))
	UImage* Icon;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Name;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* QuantityText;
};
