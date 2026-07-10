// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BurgerHUD.generated.h"

class UItemBase;
class UHighlightWidget;
class UHotbarPanel;
struct FInteractableData;
class UInteractionWidget;
class UMainMenu;

UCLASS()
class GAS_STATION_API ABurgerHUD : public AHUD
{
	GENERATED_BODY()

public:
	//=================================================================================================
	// PROPERTIES & VARIABLES
	//=================================================================================================
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainMenu> MainMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UHotbarPanel> HotbarPanelClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UHighlightWidget> HighlightWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	bool bIsMenuVisible;

	//=================================================================================================
	// FUNCTIONS
	//=================================================================================================
	ABurgerHUD();

	void DisplayMenu();
	void HideMenu();

	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;
	void UpdateInteractionWidget(const FInteractableData* InteractableData) const;

	void ShowHighlightWidget() const;
	void HideHighlightWidget() const;
	void UpdateHighlightWidget(const UItemBase* SelectedItem) const;

protected:
	//=================================================================================================
	// PROPERTIES & VARIABLES
	//=================================================================================================
	UPROPERTY()
	UMainMenu* MainMenuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;

	UPROPERTY()
	UHotbarPanel* HotbarPanelWidget;

	UPROPERTY()
	UHighlightWidget* HighlightWidget;

	UPROPERTY()
	UUserWidget* CrosshairWidget;

	//=================================================================================================
	// FUNCTIONS
	//=================================================================================================
	virtual void BeginPlay() override;
};
