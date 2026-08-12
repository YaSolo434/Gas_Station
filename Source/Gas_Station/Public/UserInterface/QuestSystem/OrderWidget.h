// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestSystem/Order/CustomerOrder.h"
#include "OrderWidget.generated.h"

class UCustomerOrderWidget;
class UTextBlock;
class UVerticalBox;

UCLASS()
class GAS_STATION_API UOrderWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* OrderContainer;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ScoreText;

	UPROPERTY(EditDefaultsOnly, Category = "Order")
	TSubclassOf<UCustomerOrderWidget> CustomerOrderWidgetClass;

	UPROPERTY()
	TMap<FGuid, TObjectPtr<UCustomerOrderWidget>> OrderWidgets;

	UFUNCTION()
	void HandleOrderSpawned(const FCustomerOrder& Order);

	UFUNCTION()
	void HandleOrderResult(const FCustomerOrder& Order, bool bSuccess);

	UFUNCTION()
	void HandleScoreChanged(int32 NewScore);

private:
	void UpdateActiveOrderWidgets();
	void AddOrderWidget(const FCustomerOrder& Order);
	void UpdateOrderWidgetVisibility();
};
