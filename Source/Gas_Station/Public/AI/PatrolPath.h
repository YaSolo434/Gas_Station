// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

UCLASS()
class GAS_STATION_API APatrolPath : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APatrolPath();

	FVector GetPatrolPoint(const int Index) const { return PatrolPoints[Index]; }
	int GetNum() const { return PatrolPoints.Num(); }

protected:
	UPROPERTY(EditAnywhere, Category = "AI", meta=(MakeEditWidget = "true", AllowPrivateAccess = "true"))
	TArray<FVector> PatrolPoints;
};
