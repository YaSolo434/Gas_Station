// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "QuestSystem/Order/CustomerOrder.h"
#include "OrderSubSystem.generated.h"

class UItemBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOrderSpawned, const FCustomerOrder&, Order);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOrderResult, const FCustomerOrder&, Order, bool, bSuccess);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);

UCLASS()
class GAS_STATION_API UOrderSubSystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Burger")
	UDataTable* ItemTable;

	UPROPERTY(EditDefaultsOnly, Category = "Burger")
	int32 PointsPerCorrectOrder = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Burger")
	int32 PointsLostPerWrongOrder = 5;

	UPROPERTY()
	FOnOrderSpawned OnOrderSpawned;

	UPROPERTY()
	FOnOrderResult OnOrderResult;

	UPROPERTY()
	FOnScoreChanged OnScoreChanged;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UOrderSubSystem, STATGROUP_Tickables); }

	FCustomerOrder GenerateRandomOrder();

	bool SubmitBurger(FGuid OrderID, UItemBase* ItemIn);

	const FItemData* GetFoodItemData(EFoodType FoodType) const { return FoodTypeLookup.Find(FoodType); }

	int32 GetScore() const { return Score; }

protected:
	UPROPERTY()
	TMap<EFoodType, FItemData> FoodTypeLookup;

	UPROPERTY()
	TArray<FCustomerOrder> ActiveOrders;

	UPROPERTY()
	int32 Score = 0;

	void ExpireOrder(FCustomerOrder& Order);
	FText BuildOrderText(const FBurgerRecipe& Recipe) const;
};
