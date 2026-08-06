// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "World/Pickup.h"
#include "CompletedBurger.generated.h"

class UBoxComponent;

UCLASS()
class GAS_STATION_API ACompletedBurger : public APickup
{
	GENERATED_BODY()

public:
	ACompletedBurger();

	UPROPERTY()
	TArray<TObjectPtr<UStaticMeshComponent>> IngredientMeshes;

	void AddIngredientMesh(UStaticMesh* Mesh);

	virtual void BeginFocus() override;
	virtual void EndFocus() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> IngredientContainer;

	UPROPERTY(EditAnywhere, Category = "Assembly | Sockets")
	FName NextIngredientSocketName = "NextIngredient_Socket";

	void ServeToCustomer(FGuid OrderID);
};
