// YaSolo
#include "Characters/Customer.h"
#include "AI/CustomerAIController.h"
#include "Characters/PlayerCharacter.h"
#include "Components/InventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/ItemBase.h"
#include "QuestSystem/OrderSubSystem.h"

// Sets default values
ACustomer::ACustomer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ACustomerAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	bUseControllerRotationYaw = false;

	LastForwardCheckTime = 0.0f;
	ForwardCheckFrequency = 0.1f;
	ForwardCheckDistance = 50.0f;
}

// Called when the game starts or when spawned
void ACustomer::BeginPlay()
{
	Super::BeginPlay();

	UpdateInteractableData();
}

// Called every frame
void ACustomer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->TimeSince(LastForwardCheckTime) > ForwardCheckFrequency)
	{
		PerformForwardCheck();
	}
}

void ACustomer::SetPatrolPath(APatrolPath* InPatrolPath, float InRestaurantPatrolIndex)
{
	PatrolPath = InPatrolPath;
	RestaurantPatrolIndex = InRestaurantPatrolIndex;
}

void ACustomer::Interact(APlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		UInventoryComponent* PlayerInventory = PlayerCharacter->GetInventory();
		UItemBase* HeldItem = PlayerInventory->GetSelectedItem();

		if (HeldItem && HeldItem->ItemType == EItemType::Food && HeldItem->FoodType == EFoodType::CompletedBurger)
		{
			UOrderSubSystem* OrderSubSystem = GetWorld()->GetSubsystem<UOrderSubSystem>();
			if (OrderSubSystem->SubmitBurger(CurrentOrderID, HeldItem))
			{
				PlayerInventory->RemoveSelectedItem();
			}
			if (const ACustomerAIController* CustomerController = Cast<ACustomerAIController>(GetController()))
			{
				CustomerController->SetBlackboardBoolValue(TEXT("HasOrdered"), false);
				CustomerController->SetBlackboardBoolValue(TEXT("CanMove"), true);
			}
		}
	}
}

void ACustomer::GenerateOrder()
{
	if (UOrderSubSystem* OrderSubSystem = GetWorld()->GetSubsystem<UOrderSubSystem>())
	{
		const FCustomerOrder Order = OrderSubSystem->GenerateRandomOrder();
		CurrentOrderID = Order.OrderID;
		CurrentOrderNum = Order.OrderNumber;
		CurrentOrderTime = Order.TimeRemaining;

		UpdateInteractableData();
	}
}

void ACustomer::PerformForwardCheck()
{
	LastForwardCheckTime = GetWorld()->GetTimeSeconds();

	const FVector TraceStart{GetActorLocation()};
	const FVector TraceEnd{TraceStart + (GetActorForwardVector() * ForwardCheckDistance)};

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FHitResult HitResult;
	bool bHitAnotherCustomer = false;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Pawn, QueryParams))
	{
		AActor* HitActor = HitResult.GetActor();

		if (HitActor && HitActor->IsA(StaticClass()))
		{
			bHitAnotherCustomer = true;
		}
	}

	const FColor DebugColor = bHitAnotherCustomer ? FColor::Red : FColor::Green;
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, DebugColor);

	if (const ACustomerAIController* CustomerController = Cast<ACustomerAIController>(GetController()))
	{
		if (!CustomerController->GetBlackBoardBoolValue(TEXT("HasOrdered")))
		{
			CustomerController->SetBlackboardBoolValue(TEXT("CanMove"), !bHitAnotherCustomer);
		}
	}
}

void ACustomer::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::NonPlayerCharacter;
	InstanceInteractableData.Name = FText::FromString("Customer No: " + FString::FromInt(CurrentOrderNum));
	InstanceInteractableData.Action = FText::FromString("Serve Burger to ");
	InstanceInteractableData.InteractionDuration = 0.f;

	InteractableData = InstanceInteractableData;
}
