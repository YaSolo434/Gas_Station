// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "RecycleBin.generated.h"

UCLASS()
class GAS_STATION_API ARecycleBin : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	//=================================================================================================                                                                                                                                 
	// PROPERTIES & VARIABLES                                                                                                                                                                                                           
	//================================================================================================= 
	ARecycleBin();

	UPROPERTY(EditAnywhere, Category = "Bin | Components")
	UStaticMeshComponent* BinMesh;

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;

	//=================================================================================================                                                                                                                                 
	// FUNCTIONS                                                                                                                                                                                                                        
	//=================================================================================================
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(APlayerCharacter* PlayerCharacter) override;
	virtual void BeginPlay() override;

	void UpdateInteractableData();
};
