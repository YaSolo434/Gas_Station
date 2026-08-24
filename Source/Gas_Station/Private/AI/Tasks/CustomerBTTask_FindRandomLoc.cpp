// YaSolo
#include "AI/Tasks/CustomerBTTask_FindRandomLoc.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UCustomerBTTask_FindRandomLoc::UCustomerBTTask_FindRandomLoc()
{
	NodeName = TEXT("Find Random Location");

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UCustomerBTTask_FindRandomLoc, BlackboardKey));
}

EBTNodeResult::Type UCustomerBTTask_FindRandomLoc::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get AI pawn
	AAIController* AIController = OwnerComp.GetAIOwner();
	const APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();

	//Get Pawn origin
	const FVector Origin{AIPawn->GetActorLocation()};

	//Get Nav system and find a random loc then move the AI to that loc
	if (const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		FNavLocation Destination;
		NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, Destination);

		AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Destination.Location);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}

FString UCustomerBTTask_FindRandomLoc::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}
