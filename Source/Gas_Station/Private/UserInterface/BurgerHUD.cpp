// YaSolo
#include "UserInterface/BurgerHUD.h"
#include "UserInterface/MainMenu.h"
#include "UserInterface/Interaction/InteractionWidget.h"
#include "UserInterface/Inventory/Hotbar/HotbarPanel.h"

ABurgerHUD::ABurgerHUD()
{
}

void ABurgerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuClass)
	{
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport(5);
			MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		if (InteractionWidget)
		{
			InteractionWidget->AddToViewport(-1);
			InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (HotbarPanelClass)
	{
		HotbarPanelWidget = CreateWidget<UHotbarPanel>(GetWorld(), HotbarPanelClass);
		if (HotbarPanelWidget)
		{
			HotbarPanelWidget->AddToViewport(-1);
			HotbarPanelWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ABurgerHUD::DisplayMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABurgerHUD::HideMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ABurgerHUD::ShowInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABurgerHUD::HideInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ABurgerHUD::UpdateInteractionWidget(const FInteractableData* InteractableData) const
{
	ShowInteractionWidget();

	if (InteractionWidget)
	{
		InteractionWidget->UpdateWidget(InteractableData);
	}
}
