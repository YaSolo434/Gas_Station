#pragma once

#include "CoreMinimal.h"
#include "Data/ItemDataStruct.h"
#include "BurgerOrder.generated.h"

USTRUCT()
struct FBurgerRecipe
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<EFoodType, int32> Ingredients;

	bool operator==(const FBurgerRecipe& Other) const
	{
		if (Ingredients.Num() != Other.Ingredients.Num())
		{
			return false;
		}

		for (const auto& Pair : Ingredients)
		{
			const int32* OtherCount = Other.Ingredients.Find(Pair.Key);
			if (!OtherCount || *OtherCount != Pair.Value)
			{
				return false;
			}
		}
		return true;
	}
};
