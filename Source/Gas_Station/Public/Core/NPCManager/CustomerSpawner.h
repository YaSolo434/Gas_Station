// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomerSpawner.generated.h"

class APatrolPath;
class ACustomer;

UCLASS()
class GAS_STATION_API ACustomerSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACustomerSpawner();

	UPROPERTY(EditAnywhere, Category="Spawning")
	TSubclassOf<ACustomer> CustomerClass;

	UPROPERTY(EditAnywhere, Category="Spawning")
	float MaxSpawnInterval = 60.0f;

	UPROPERTY(EditAnywhere, Category="Spawning")
	float MinSpawnInterval = 10.0f;

	UPROPERTY(EditInstanceOnly, Category="Spawning")
	APatrolPath* AssignedPatrolPath;

	UPROPERTY(EditInstanceOnly, Category="Spawning")
	int32 RestaurantPatrolIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	float SpawnInterval;

	void HandleCustomerSpawning();
	void SpawnCustomer();

	FTimerHandle SpawnTimerHandle;
};
