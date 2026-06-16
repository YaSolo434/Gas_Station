// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"

#include "Pickup.generated.h"

class UItemBase;

UCLASS()
class GAS_STATION_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	//=================================================================================================
	// PROPERTIES & VARIABLES
	//=================================================================================================

	//=================================================================================================
	// FUNCTIONS
	//=================================================================================================
	APickup();

	void InitializePickup(const TSubclassOf<UItemBase> BaseClass);
	void InitializeDrop(UItemBase* ItemToDrop);

	FORCEINLINE UItemBase* GetItemData() const { return ItemReference; }

	virtual void BeginFocus() override;
	virtual void EndFocus() override;

protected:
	//=================================================================================================
	// PROPERTIES & VARIABLES
	//=================================================================================================
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	FName DesiredItemID;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference;

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;

	//=================================================================================================
	// FUNCTIONS
	//=================================================================================================
	virtual void BeginPlay() override;

	virtual void Interact(APlayerCharacter* PlayerCharacter) override;
	void UpdateInteractableData();

	UFUNCTION()
	void TakePickup(const APlayerCharacter* Taker);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
