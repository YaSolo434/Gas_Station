// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BurgerHUD.generated.h"

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

	//=================================================================================================
	// FUNCTIONS
	//=================================================================================================
	virtual void BeginPlay() override;
};
