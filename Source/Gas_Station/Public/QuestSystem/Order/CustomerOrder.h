#pragma once
#include "CoreMinimal.h"
#include "BurgerOrder.h"
#include "CustomerOrder.generated.h"

USTRUCT()
struct FCustomerOrder
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid OrderID;

	UPROPERTY()
	FBurgerRecipe Recipe;

	UPROPERTY()
	FText OrderText;

	UPROPERTY()
	float TimeRemaining = 0.f;

	UPROPERTY()
	float MaxTime = 50.f;

	UPROPERTY()
	float MinTime = 10.f;
};
