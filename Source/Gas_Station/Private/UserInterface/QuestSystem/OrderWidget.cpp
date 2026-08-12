// YaSolo


#include "UserInterface/QuestSystem/OrderWidget.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameFramework/PlayerController.h"
#include "QuestSystem/OrderSubSystem.h"
#include "UserInterface/BurgerHUD.h"
#include "UserInterface/QuestSystem/CustomerOrderWidget.h"

void UOrderWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("UOrderWidget::NativeConstruct"));

	UOrderSubSystem* OrderSubSystem = GetWorld()->GetSubsystem<UOrderSubSystem>();

	if (!OrderSubSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("UOrderSubSystem is NULL"));
		return;
	}

	OrderSubSystem->OnOrderSpawned.AddDynamic(this, &UOrderWidget::HandleOrderSpawned);
	OrderSubSystem->OnOrderResult.AddDynamic(this, &UOrderWidget::HandleOrderResult);
	OrderSubSystem->OnScoreChanged.AddDynamic(this, &UOrderWidget::HandleScoreChanged);

	if (ScoreText)
	{
		ScoreText->SetText(FText::AsNumber(OrderSubSystem->GetScore()));
	}

	UpdateActiveOrderWidgets();
	UpdateOrderWidgetVisibility();
}

void UOrderWidget::NativeDestruct()
{
	if (UOrderSubSystem* OrderSubSystem = GetWorld()->GetSubsystem<UOrderSubSystem>())
	{
		OrderSubSystem->OnOrderSpawned.RemoveDynamic(this, &UOrderWidget::HandleOrderSpawned);
		OrderSubSystem->OnOrderResult.RemoveDynamic(this, &UOrderWidget::HandleOrderResult);
		OrderSubSystem->OnScoreChanged.RemoveDynamic(this, &UOrderWidget::HandleScoreChanged);
	}

	Super::NativeDestruct();
}

void UOrderWidget::HandleOrderSpawned(const FCustomerOrder& Order)
{
	AddOrderWidget(Order);
	UpdateOrderWidgetVisibility();
}

void UOrderWidget::HandleOrderResult(const FCustomerOrder& Order, bool bSuccess)
{
	if (const TObjectPtr<UCustomerOrderWidget>* FoundWidget = OrderWidgets.Find(Order.OrderID))
	{
		if (*FoundWidget)
		{
			(*FoundWidget)->RemoveFromParent();
		}
		OrderWidgets.Remove(Order.OrderID);
	}
	UpdateOrderWidgetVisibility();
}

void UOrderWidget::HandleScoreChanged(int32 NewScore)
{
	if (ScoreText)
	{
		ScoreText->SetText(FText::AsNumber(NewScore));
	}
}

void UOrderWidget::UpdateActiveOrderWidgets()
{
	const UOrderSubSystem* OrderSubSystem = GetWorld()->GetSubsystem<UOrderSubSystem>();

	if (OrderSubSystem)
	{
		for (const FCustomerOrder& Order : OrderSubSystem->GetActiveOrders())
		{
			AddOrderWidget(Order);
		}
	}
}

void UOrderWidget::AddOrderWidget(const FCustomerOrder& Order)
{
	if (!CustomerOrderWidgetClass || !OrderContainer || OrderWidgets.Contains(Order.OrderID))
	{
		return;
	}

	UCustomerOrderWidget* OrderWidget = CreateWidget<UCustomerOrderWidget>(this, CustomerOrderWidgetClass);
	if (OrderWidget)
	{
		OrderWidget->SetupOrder(Order);
		OrderContainer->AddChildToVerticalBox(OrderWidget);
		OrderWidgets.Add(Order.OrderID, OrderWidget);
	}
}

void UOrderWidget::UpdateOrderWidgetVisibility()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ABurgerHUD* HUD = Cast<ABurgerHUD>(PC->GetHUD()))
		{
			if (UOrderSubSystem* OrderSubSystem = GetWorld()->GetSubsystem<UOrderSubSystem>())
			{
				if (OrderSubSystem->GetActiveOrders().Num() >= 1)
				{
					HUD->ShowOrderWidget();
				}
				else
				{
					HUD->HideOrderWidget();
				}
			}
		}
	}
}
