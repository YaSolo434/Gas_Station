// YaSolo
#include "AI/Tasks/BTTask_TurnToPlayer.h"

#include "AI/CustomerAIController.h"
#include "Characters/Customer.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_TurnToPlayer::UBTTask_TurnToPlayer(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Turn to Player");
}

EBTNodeResult::Type UBTTask_TurnToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const ACustomerAIController* CustomerController = Cast<ACustomerAIController>(OwnerComp.GetAIOwner()))
	{
		//getting customer ai 
		if (ACustomer* Customer = Cast<ACustomer>(CustomerController->GetPawn()))
		{
			//getting player via customer
			const APlayerCharacter* Player = Cast<
				APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(Customer->GetWorld(), 0));

			if (Player)
			{
				const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
					Customer->GetActorLocation(), Player->GetActorLocation());
				const FRotator TargetYawRotation = FRotator(0.0f, LookAtRotation.Yaw, 0.0f);

				Customer->SetActorRotation(TargetYawRotation);

				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
