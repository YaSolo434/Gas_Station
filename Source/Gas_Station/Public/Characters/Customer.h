// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/InteractionInterface.h"
#include "Customer.generated.h"

class APatrolPath;
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

	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

	APatrolPath* GetPatrolPath() const { return PatrolPath; }

	virtual void Interact(APlayerCharacter* PlayerCharacter) override;

	UPROPERTY(EditAnywhere, Category = "AI")
	int32 RestaurantPatrolIndex;

	void GenerateOrder();

	float CurrentOrderTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(VisibleInstanceOnly, Category = "Order")
	FGuid CurrentOrderID;

	UPROPERTY(VisibleDefaultsOnly, Category = "Order")
	int32 CurrentOrderNum;

	UPROPERTY(VisibleInstanceOnly, Category = "Order")
	FInteractableData InstanceInteractableData;

	UPROPERTY(EditAnywhere, Category = "AI")
	APatrolPath* PatrolPath;

private:
	void UpdateInteractableData();
};
