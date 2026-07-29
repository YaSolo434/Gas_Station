// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "PreparationStation.generated.h"

class ACompletedBurger;
class UItemBase;
class UBoxComponent;

UENUM()
enum class EIngredientAddResult : uint8
{
	IAR_IngredientAdded UMETA(DisplayName = "Ingredient Added"),
	IAR_FirstIngredientIsNotBread UMETA(DisplayName = "First Ingredient is not bottom bread")
};

UCLASS()
class GAS_STATION_API APreparationStation : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	//=================================================================================================                                                                                                                                 
	// PROPERTIES & VARIABLES                                                                                                                                                                                                           
	//================================================================================================= 
	APreparationStation();

protected:
	//=================================================================================================                                                                                                                                 
	// PROPERTIES & VARIABLES                                                                                                                                                                                                           
	//=================================================================================================
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> WorkSpace;

	UPROPERTY(VisibleAnywhere, Category = "Assembly")
	TArray<TObjectPtr<UStaticMeshComponent>> AssembledMeshes;

	UPROPERTY(VisibleAnywhere, Category = "Assembly")
	int IngredientCount = 0;

	UPROPERTY(VisibleAnywhere, Category = "Assembly")
	bool bHasTopBread = false;

	UPROPERTY(VisibleAnywhere, Category = "Assembly")
	bool bHasBottomBread = false;

	UPROPERTY(VisibleAnywhere, Category = "Assembly")
	bool bIsComplete = false;

	//socket names
	UPROPERTY(EditAnywhere, Category = "Assembly | Sockets")
	FName NextIngredientSocketName = "NextIngredient_Socket";

	UPROPERTY(EditAnywhere, Category = "Assembly")
	TSubclassOf<ACompletedBurger> CompletedBurgerClass;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	FName DesiredItemID;

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Item Database")
	FInteractableData InstanceInteractableData;

	//=================================================================================================                                                                                                                                 
	// FUNCTIONS                                                                                                                                                                                                                        
	//=================================================================================================
	virtual void BeginPlay() override;

	// Interaction Interface
	virtual void Interact(APlayerCharacter* PlayerCharacter) override;
	virtual void BeginFocus() override;
	virtual void EndFocus() override;

	//Assembly func
	UFUNCTION(Category = "Assembly")
	bool AddIngredient(const UItemBase* Ingredient);

	UFUNCTION(Category = "Assembly")
	void ClearAssembly();

	UFUNCTION(Category = "Assembly")
	bool CanAddIngredient(const UItemBase* Ingredient) const;

	UFUNCTION(Category = "Assembly")
	bool IsAssemblyComplete() const { return bIsComplete; };

private:
	FVector CalculateFirstIngredientPosition() const;
	void UpdateAssemblyState(const UItemBase* Ingredient);
	void UpdateInteractableData();
	void OnAssemblyComplete();
	bool IsBreadBottom(const UItemBase* Ingredient) const;
	bool IsBreadTop(const UItemBase* Ingredient) const;
	UStaticMeshComponent* GetLastPlacedMesh() const;
};
