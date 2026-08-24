// YaSolo
#include "AI/Tasks/CustomerBTTask_FindPathPoint.h"

#include "AI/CustomerAIController.h"
#include "AI/PatrolPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Customer.h"

UCustomerBTTask_FindPathPoint::UCustomerBTTask_FindPathPoint(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Find Path Point");
}

EBTNodeResult::Type UCustomerBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const ACustomerAIController* Controller = Cast<ACustomerAIController>(OwnerComp.GetAIOwner()))
	{
		if (UBlackboardComponent* BC = OwnerComp.GetBlackboardComponent())
		{
			const int32 Index = BC->GetValueAsInt(GetSelectedBlackboardKey());

			if (const ACustomer* Customer = Cast<ACustomer>(Controller->GetPawn()))
			{
				const FVector LocalPoint = Customer->GetPatrolPath()->GetPatrolPoint(Index);
				const FVector GlobalPoint = Customer->GetPatrolPath()->GetActorTransform().TransformPosition(LocalPoint);

				BC->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);

				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}
