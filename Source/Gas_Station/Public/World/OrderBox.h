// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "OrderBox.generated.h"

class APlayerCharacter;
class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class GAS_STATION_API AOrderBox : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	AOrderBox();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> BoxMesh;

	UPROPERTY(VisibleInstanceOnly, Category = "Order")
	FGuid CurrentOrderID;

	UPROPERTY(VisibleInstanceOnly, Category = "Order")
	FInteractableData InstanceInteractableData;

	UPROPERTY(EditAnywhere, Category = "Order")
	float OrderSpawnDelaySeconds = 5.f;

	FTimerHandle OrderTimerHandle;

	virtual void Interact(APlayerCharacter* PlayerCharacter) override;
	virtual void BeginFocus() override;
	virtual void EndFocus() override;

private:
	void GenerateNextOrder();
	bool IsCurrentOrderActive() const;
	void UpdateInteractableData();
};
