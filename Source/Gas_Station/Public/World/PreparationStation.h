// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "PreparationStation.generated.h"

class UItemBase;
class UBoxComponent;

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

	//=================================================================================================                                                                                                                                 
	// FUNCTIONS                                                                                                                                                                                                                        
	//=================================================================================================
	virtual void BeginPlay() override;

	virtual void Interact(APlayerCharacter* PlayerCharacter) override;

	//Assembly func
	UFUNCTION(Category = "Assembly")
	bool AddIngredient(UItemBase* Ingredient);

	UFUNCTION(Category = "Assembly")
	void ClearAssembly()

	UFUNCTION(Category = "Assembly")
	void PickupCompletedBurger(APlayerCharacter* PlayerCharacter);

	UFUNCTION(Category = "Assembly")
	bool CanAddIngredient(const UItemBase* Ingredient) const;

	UFUNCTION(Category = "Assembly")
	bool IsAssemblyComplete() const { return bIsComplete; };

private:
	FVector CalculateFirstIngredientPosition() const;
	void UpdateAssemblyState(const UItemBase* Ingredient);
	bool IsBreadBottom(const UItemBase* Ingredient) const;
	bool IsBreadTop(const UItemBase* Ingredient) const;
	UStaticMeshComponent* GetLastPlacedMesh() const;
};
