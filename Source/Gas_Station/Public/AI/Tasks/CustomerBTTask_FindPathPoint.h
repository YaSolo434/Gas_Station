// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CustomerBTTask_FindPathPoint.generated.h"

UCLASS()
class GAS_STATION_API UCustomerBTTask_FindPathPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UCustomerBTTask_FindPathPoint(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector PatrolPathVectorKey;
};
