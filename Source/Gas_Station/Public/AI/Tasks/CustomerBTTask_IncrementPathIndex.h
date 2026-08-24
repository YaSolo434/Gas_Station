// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CustomerBTTask_IncrementPathIndex.generated.h"


UCLASS()
class GAS_STATION_API UCustomerBTTask_IncrementPathIndex : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UCustomerBTTask_IncrementPathIndex(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
