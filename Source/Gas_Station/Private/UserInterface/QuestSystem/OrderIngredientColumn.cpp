// YaSolo
#include "UserInterface/QuestSystem/OrderIngredientColumn.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "QuestSystem/OrderSubSystem.h"

void UOrderIngredientColumn::SetIngredient(EFoodType FoodType, int32 Quantity)
{
	if (!Icon || !QuantityText || !Name)
	{
		return;
	}

	const UOrderSubSystem* OrderSubSystem = GetWorld()->GetSubsystem<UOrderSubSystem>();
	if (OrderSubSystem)
	{
		const FItemData* ItemData = OrderSubSystem->GetFoodItemData(FoodType);
		if (ItemData)
		{
			if (ItemData->AssetData.Icon)
			{
				Icon->SetBrushFromTexture(ItemData->AssetData.Icon);
				Icon->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				Icon->SetVisibility(ESlateVisibility::Collapsed);
			}

			Name->SetText(ItemData->DescriptiveText.DisplayName);

			if (Quantity > 1)
			{
				QuantityText->SetText(FText::FromString(FString::Printf(TEXT("x%d"), Quantity)));
				QuantityText->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				QuantityText->SetText(FText::GetEmpty());
				QuantityText->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		else
		{
			SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
