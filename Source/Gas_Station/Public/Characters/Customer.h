// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/InteractionInterface.h"
#include "Customer.generated.h"

struct FCustomerOrder;
class UBehaviorTree;

UCLASS()
class GAS_STATION_API ACustomer : public ACharacter, public IInteractionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACustomer();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }

	virtual void Interact(APlayerCharacter* PlayerCharacter) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(VisibleInstanceOnly, Category = "Order")
	FGuid CurrentOrderID;

	UPROPERTY(VisibleInstanceOnly, Category = "Order")
	FInteractableData InstanceInteractableData;

private:
	FTimerHandle DelayTimerHandle;

	void GenerateNextOrder();
	bool IsCurrentOrderActive() const;
	void UpdateInteractableData();
};
