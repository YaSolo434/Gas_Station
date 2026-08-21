// YaSolo
#include "AI/CustomerAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Characters/Customer.h"

ACustomerAIController::ACustomerAIController()
{
}

void ACustomerAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ACustomerAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ACustomer* const NPC = Cast<ACustomer>(InPawn))
	{
		if (UBehaviorTree* const BT = NPC->GetBehaviorTree())
		{
			RunBehaviorTree(BT);
		}
	}
}
