// YaSolo
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "CustomerAIController.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class GAS_STATION_API ACustomerAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACustomerAIController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
};
