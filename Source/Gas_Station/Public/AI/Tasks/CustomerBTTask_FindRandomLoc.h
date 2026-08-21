// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CustomerBTTask_FindRandomLoc.generated.h"
UCLASS()
class GAS_STATION_API UCustomerBTTask_FindRandomLoc : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UCustomerBTTask_FindRandomLoc();

private:
	virtual FString GetStaticDescription() const override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere, Category = "Search")
	float SearchRadius {500.f};
};
