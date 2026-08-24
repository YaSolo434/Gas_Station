// YaSolo
#include "AI/Tasks/CustomerBTTask_IncrementPathIndex.h"
#include "AI/CustomerAIController.h"
#include "AI/PatrolPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Customer.h"

UCustomerBTTask_IncrementPathIndex::UCustomerBTTask_IncrementPathIndex(const FObjectInitializer& ObjectInitializer) :
	UBTTask_BlackboardBase{ObjectInitializer}
{
	NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UCustomerBTTask_IncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const ACustomerAIController* Controller = Cast<ACustomerAIController>(OwnerComp.GetAIOwner()))
	{
		if (ACustomer* Customer = Cast<ACustomer>(Controller->GetPawn()))
		{
			if (UBlackboardComponent* BC = OwnerComp.GetBlackboardComponent())
			{
				//Get max number of patrol points from Enemy
				const int NumberOfPoints = Customer->GetPatrolPath()->GetNum();
				const int MaxIndex = NumberOfPoints - 1;

				//Get current index
				int Index = BC->GetValueAsInt(GetSelectedBlackboardKey());

				if (Index >= MaxIndex)
				{
					Customer->Destroy();
				}

				if (Index == Customer->RestaurantPatrolIndex)
				{
					// BC->SetValueAsInt(GetSelectedBlackboardKey(), ++Index);

					Customer->GenerateOrder();
					BC->SetValueAsFloat(TEXT("WaitTime"), Customer->CurrentOrderTime + 2.0f);

					BC->SetValueAsBool(TEXT("HasOrdered"), true);
					BC->SetValueAsBool(TEXT("CanMove"), false);
				}

				//update current index
				BC->SetValueAsInt(GetSelectedBlackboardKey(), ++Index);

				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
