// YaSolo

#include "QuestSystem/OrderSubSystem.h"

#include "Items/ItemBase.h"

void UOrderSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SetItemTable(ItemTable);
}

void UOrderSubSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int32 i = ActiveOrders.Num() - 1; i >= 0; --i)
	{
		ActiveOrders[i].TimeRemaining -= DeltaTime;
		if (ActiveOrders[i].TimeRemaining <= 0.f)
		{
			ExpireOrder(ActiveOrders[i]);
			ActiveOrders.RemoveAt(i);
		}
	}
}

void UOrderSubSystem::SetItemTable(const UDataTable* InItemTable)
{
	FoodTypeLookup.Empty();

	if (InItemTable)
	{
		TArray<FItemData*> Rows;
		InItemTable->GetAllRows<FItemData>(TEXT("OrderSubSystem::SetItemTable"), Rows);

		for (const FItemData* Row : Rows)
		{
			if (Row->ItemType == EItemType::Food &&
				Row->FoodType != EFoodType::None &&
				Row->FoodType != EFoodType::CompletedBurger)
			{
				FoodTypeLookup.Add(Row->FoodType, *Row);
			}
		}
	}
}

FCustomerOrder UOrderSubSystem::GenerateRandomOrder()
{
	UE_LOG(LogTemp, Warning, TEXT("GenerateRandomOrder()"));

	FCustomerOrder Order;
	Order.OrderID = FGuid::NewGuid();
	Order.TimeRemaining = FMath::RandRange(Order.MinTime, Order.MaxTime);


	Order.Recipe.Ingredients.Add(EFoodType::BreadBottom, 1);
	Order.Recipe.Ingredients.Add(EFoodType::BreadTop, 1);

	TArray<EFoodType> Ingredients;
	for (const auto& Pair : FoodTypeLookup)
	{
		EFoodType Type = Pair.Key;
		if (Type == EFoodType::BreadBottom || Type == EFoodType::BreadTop)
		{
			continue;
		}
		Ingredients.Add(Type);
	}

	const int32 NumIngredients = FMath::RandRange(1, Ingredients.Num());
	for (int32 i = 0; i < NumIngredients && Ingredients.Num() > 0; ++i)
	{
		const int32 Index = FMath::RandRange(0, Ingredients.Num() - 1);
		Order.Recipe.Ingredients.Add(Ingredients[Index], 1);
		Ingredients.RemoveAt(Index);
	}

	Order.OrderText = BuildOrderText(Order.Recipe);
	ActiveOrders.Add(Order);
	OnOrderSpawned.Broadcast(Order);

	return Order;
}

bool UOrderSubSystem::SubmitBurger(FGuid OrderID, UItemBase* ItemIn)
{
	if (!ItemIn)
	{
		return false;
	}

	FBurgerRecipe SubmittedRecipe;
	for (EFoodType FoodType : ItemIn->IngredientTypes)
	{
		int32& Count = SubmittedRecipe.Ingredients.FindOrAdd(FoodType);
		Count++;
	}

	for (int32 i = 0; i < ActiveOrders.Num(); ++i)
	{
		if (ActiveOrders[i].OrderID == OrderID)
		{
			FCustomerOrder Order = ActiveOrders[i];
			bool bCorrect = Order.Recipe == SubmittedRecipe;

			Score += bCorrect ? PointsPerCorrectOrder : -PointsLostPerWrongOrder;
			Score = FMath::Max(Score, 0);

			OnOrderResult.Broadcast(Order, bCorrect);
			OnScoreChanged.Broadcast(Score);

			ActiveOrders.RemoveAt(i);

			return bCorrect;
		}
	}

	return false;
}

void UOrderSubSystem::ExpireOrder(FCustomerOrder& Order)
{
	Score -= PointsLostPerWrongOrder;
	Score = FMath::Max(Score, -20);

	OnOrderResult.Broadcast(Order, false);
	OnScoreChanged.Broadcast(Score);
}

FText UOrderSubSystem::BuildOrderText(const FBurgerRecipe& Recipe) const
{
	TArray<FString> Parts;

	for (const auto& Pair : Recipe.Ingredients)
	{
		if (Pair.Key == EFoodType::BreadBottom || Pair.Key == EFoodType::BreadTop)
		{
			continue;
		}

		const FItemData* Item = FoodTypeLookup.Find(Pair.Key);

		if (Item)
		{
			FString Name = Item->DescriptiveText.DisplayName.ToString();

			if (Pair.Value > 1)
			{
				Name = FString::Printf(TEXT("%s, x%d"), *Name, Pair.Value);
			}
			Parts.Add(Name);
		}
	}

	return FText::FromString(FString::Printf(TEXT("I'd like a burger with %s, please!"),
	                                         *FString::Join(Parts, TEXT(", "))));
}
