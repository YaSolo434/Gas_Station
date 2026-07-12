// YaSolo
#include "UserInterface/BurgerHUD.h"
#include "UserInterface/MainMenu.h"
#include "UserInterface/Interaction/InteractionWidget.h"
#include "UserInterface/Inventory/HighlightWidget.h"
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
			HotbarPanelWidget->AddToViewport(4);
			HotbarPanelWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}

	if (HighlightWidgetClass)
	{
		HighlightWidget = CreateWidget<UHighlightWidget>(GetWorld(), HighlightWidgetClass);
		if (HighlightWidget)
		{
			HighlightWidget->AddToViewport(-1);
			HighlightWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (CrosshairWidgetClass)
	{
		CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
		if (CrosshairWidget)
		{
			CrosshairWidget->AddToViewport(4);
			CrosshairWidget->SetVisibility(ESlateVisibility::Visible);
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

void ABurgerHUD::ShowHighlightWidget() const
{
	if (HighlightWidget)
	{
		HighlightWidget->PlayFadeIn();
		HighlightWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABurgerHUD::HideHighlightWidget() const
{
	if (HighlightWidget)
	{
		HighlightWidget->PlayFadeOut();
	}
}

void ABurgerHUD::UpdateHighlightWidget(const UItemBase* SelectedItem) const
{
	ShowHighlightWidget();

	if (HighlightWidget)
	{
		HighlightWidget->UpdateWidget(SelectedItem);
	}
}
