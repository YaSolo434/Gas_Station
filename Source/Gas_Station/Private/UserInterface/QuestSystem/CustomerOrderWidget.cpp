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

		OrderNumber->SetText(FText::FromString(FString::FromInt(Order.OrderNumber)));

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
	FNumberFormattingOptions FormattingOptions;
	FormattingOptions.MinimumFractionalDigits = 1; // Always show at least 2 decimals
	FormattingOptions.MaximumFractionalDigits = 1; // Cap at 2 decimals

	if (TimeRemaining <= 10.0f)
	{
		TimeRemainingText->SetText(FText::AsNumber(TimeRemaining, &FormattingOptions));
		if (TimeRemaining <= 5.0f)
		{
			// Sine wave math bounces between 0.0 and 1.0 over time
			const float GameTime = GetWorld()->GetTimeSeconds();
			constexpr float FlashFrequency = 10.0f;
			const float Alpha = (FMath::Sin(GameTime * FlashFrequency) + 1.0f) * 0.5f;

			// Linear interpolate between Red and White based on the sine wave
			const FLinearColor FlashingColor = FLinearColor::LerpUsingHSV(FLinearColor::Black, FLinearColor::Red, Alpha);

			TimeRemainingText->SetColorAndOpacity(FlashingColor);
		}
	}
	else
	{
		TimeRemainingText->SetText(FText::FromString(FString::FromInt(TimeRemaining)));
	}
}
