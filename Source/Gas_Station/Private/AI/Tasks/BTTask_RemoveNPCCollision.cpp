// YaSolo
#include "AI/Tasks/BTTask_RemoveNPCCollision.h"

#include "AI/CustomerAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

UMyBTTask_RemoveNPCCollision::UMyBTTask_RemoveNPCCollision(const FObjectInitializer& OI)
{
	NodeName = TEXT("Remove Collision");
}

EBTNodeResult::Type UMyBTTask_RemoveNPCCollision::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const ACustomerAIController* AICont = Cast<ACustomerAIController>(OwnerComp.GetAIOwner()))
	{
		AICont->GetCharacter()->GetCapsuleComponent()->SetCollisionResponseToChannel
			(ECC_GameTraceChannel1, ECR_Ignore);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
