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

void ACustomer::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::NonPlayerCharacter;
	InstanceInteractableData.Name = FText::FromString("Customer No: " + FString::FromInt(CurrentOrderNum));
	InstanceInteractableData.Action = FText::FromString("Serve Burger to ");
	InstanceInteractableData.InteractionDuration = 0.f;

	InteractableData = InstanceInteractableData;
}
