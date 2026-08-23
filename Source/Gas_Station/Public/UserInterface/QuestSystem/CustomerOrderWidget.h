// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestSystem/Order/CustomerOrder.h"
#include "CustomerOrderWidget.generated.h"

class UTextBlock;
class UOrderIngredientColumn;
class UHorizontalBox;

UCLASS()
class GAS_STATION_API UCustomerOrderWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetupOrder(const FCustomerOrder& Order);
	void UpdateTimeRemaining(float TimeRemaining);

protected:
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* IngredientBox;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* TimeRemainingText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* OrderNumber;

	UPROPERTY(EditDefaultsOnly, Category = "Order")
	TSubclassOf<UOrderIngredientColumn> IngredientColumnClass;
};
