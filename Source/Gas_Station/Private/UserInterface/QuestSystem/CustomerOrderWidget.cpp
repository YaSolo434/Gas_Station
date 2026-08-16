// YaSolo
#include "UserInterface/QuestSystem/CustomerOrderWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "UserInterface/QuestSystem/OrderIngredientColumn.h"

void UCustomerOrderWidget::SetupOrder(const FCustomerOrder& Order)
{
	if (IngredientBox)
	{
		IngredientBox->ClearChildren();

		TArray<EFoodType> IngredientTypes;
		Order.Recipe.Ingredients.GetKeys(IngredientTypes);

		IngredientTypes.Sort([](const EFoodType A, const EFoodType B)
		{
			return static_cast<uint8>(A) < static_cast<uint8>(B);
		});

		for (const EFoodType FoodType : IngredientTypes)
		{
			if (FoodType == EFoodType::BreadBottom || FoodType == EFoodType::BreadTop)
			{
				continue;
			}

			const int32* Quantity = Order.Recipe.Ingredients.Find(FoodType);
			if (!Quantity || !IngredientColumnClass)
			{
				continue;
			}

			UOrderIngredientColumn* Column = CreateWidget<UOrderIngredientColumn>(this, IngredientColumnClass);
			if (!Column)
			{
				continue;
			}

			Column->SetIngredient(FoodType, *Quantity);
			IngredientBox->AddChildToHorizontalBox(Column);
		}
	}
	UpdateTimeRemaining(Order.TimeRemaining);
}

void UCustomerOrderWidget::UpdateTimeRemaining(float TimeRemaining)
{
	const int32 Seconds = FMath::Max(0, FMath::CeilToInt(TimeRemaining));

	TimeRemainingText->SetText(FText::FromString(FString::FromInt(Seconds)));
}
