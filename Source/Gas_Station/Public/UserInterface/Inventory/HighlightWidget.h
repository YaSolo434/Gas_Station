// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HighlightWidget.generated.h"

class UItemBase;
class UTextBlock;

UCLASS()
class GAS_STATION_API UHighlightWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateWidget(const UItemBase* SelectedItem) const;

	UFUNCTION(Category= "UI Animations")
	void PlayFadeIn();

	UFUNCTION(Category= "UI Animations")
	void PlayFadeOut();

	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* NameText;

	UPROPERTY(Transient, meta = (BindWidgetAnim), BlueprintReadOnly, Category = "UI Animations")
	UWidgetAnimation* FadeInAnim;

	bool bIsFadingAnimReverse = false;
};
