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

protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* NameText;
};
