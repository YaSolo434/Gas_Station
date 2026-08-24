// YaSolo
#include "Core/NPCManager/CustomerSpawner.h"
#include "Characters/Customer.h"

// Sets default values
ACustomerSpawner::ACustomerSpawner()
{
}

// Called when the game starts or when spawned
void ACustomerSpawner::BeginPlay()
{
	Super::BeginPlay();

	HandleCustomerSpawning();
}

void ACustomerSpawner::HandleCustomerSpawning()
{
	SpawnInterval = FMath::RandRange(MinSpawnInterval, MaxSpawnInterval);

	GetWorldTimerManager().SetTimer(SpawnTimerHandle,
	                                this,
	                                &ACustomerSpawner::SpawnCustomer,
	                                SpawnInterval,
	                                false);
}

void ACustomerSpawner::SpawnCustomer()
{
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

	if (CustomerClass)
	{
		if (UWorld* World = GetWorld())
		{
			const FVector SpawnLocation{GetActorLocation()};
			const FRotator SpawnRotation{GetActorRotation()};

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			SpawnParams.Owner = this;

			ACustomer* SpawnedCustomer = World->SpawnActor<ACustomer>(CustomerClass, SpawnLocation, SpawnRotation, SpawnParams);

			if (SpawnedCustomer && AssignedPatrolPath)
			{
				SpawnedCustomer->SetPatrolPath(AssignedPatrolPath, RestaurantPatrolIndex);
			}

			HandleCustomerSpawning();
		}
	}
}
