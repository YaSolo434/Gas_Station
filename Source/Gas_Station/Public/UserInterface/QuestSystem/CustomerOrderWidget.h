// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestSystem/Order/CustomerOrder.h"
#include "CustomerOrderWidget.generated.h"

class UOrderIngredientColumn;
class UHorizontalBox;

UCLASS()
class GAS_STATION_API UCustomerOrderWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetupOrder(const FCustomerOrder& Order);

protected:
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* IngredientBox;

	UPROPERTY(EditDefaultsOnly, Category = "Order")
	TSubclassOf<UOrderIngredientColumn> IngredientColumnClass;
};
