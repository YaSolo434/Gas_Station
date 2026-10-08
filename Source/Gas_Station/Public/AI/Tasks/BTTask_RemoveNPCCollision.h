// YaSolo
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RemoveNPCCollision.generated.h"

UCLASS()
class GAS_STATION_API UMyBTTask_RemoveNPCCollision : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UMyBTTask_RemoveNPCCollision(const FObjectInitializer& OI);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
