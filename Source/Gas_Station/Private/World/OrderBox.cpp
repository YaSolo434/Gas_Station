// YaSolo

#include "World/OrderBox.h"

#include "Characters/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Items/ItemBase.h"
#include "QuestSystem/OrderSubSystem.h"

// Sets default values
AOrderBox::AOrderBox()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);
	Box->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Box->SetBoxExtent(FVector(50.f, 50.f, 50.f));

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	BoxMesh->SetupAttachment(Box);
	BoxMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AOrderBox::BeginPlay()
{
	Super::BeginPlay();

	UpdateInteractableData();

	GetWorldTimerManager().SetTimer(
		OrderTimerHandle,
		this,
		&AOrderBox::GenerateNextOrder,
		OrderSpawnDelaySeconds,
		false);
}

void AOrderBox::GenerateNextOrder()
{
	if (UOrderSubSystem* OrderSubSystem = GetWorld()->GetSubsystem<UOrderSubSystem>())
	{
		const FCustomerOrder Order = OrderSubSystem->GenerateRandomOrder();
		CurrentOrderID = Order.OrderID;
	}
}

bool AOrderBox::IsCurrentOrderActive() const
{
	const UOrderSubSystem* OrderSubSystem = GetWorld()->GetSubsystem<UOrderSubSystem>();

	if (!OrderSubSystem || !CurrentOrderID.IsValid())
	{
		return false;
	}

	for (const FCustomerOrder& ActiveOrder : OrderSubSystem->GetActiveOrders())
	{
		if (ActiveOrder.OrderID == CurrentOrderID)
		{
			return true;
		}
	}

	return false;
}

void AOrderBox::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::Device;
	InstanceInteractableData.Name = FText::FromString("Order Box");
	InstanceInteractableData.Action = FText::FromString("Serve burger to");
	InstanceInteractableData.InteractionDuration = 0.f;

	InteractableData = InstanceInteractableData;
}

void AOrderBox::Interact(APlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return;
	}

	if (!IsCurrentOrderActive())
	{
		GenerateNextOrder();
		UpdateInteractableData();
		return;
	}

	UInventoryComponent* PlayerInventory = PlayerCharacter->GetInventory();
	if (!PlayerInventory)
	{
		return;
	}

	UItemBase* HeldItem = PlayerInventory->GetSelectedItem();
	if (HeldItem && HeldItem->ItemType == EItemType::Food && HeldItem->FoodType == EFoodType::CompletedBurger)
	{
		if (UOrderSubSystem* OrderSubSystem = GetWorld()->GetSubsystem<UOrderSubSystem>())
		{
			if (OrderSubSystem->SubmitBurger(CurrentOrderID, HeldItem))
			{
				PlayerInventory->RemoveSelectedItem();
			}
		}

		GenerateNextOrder();
		UpdateInteractableData();
	}
}

void AOrderBox::BeginFocus()
{
	if (BoxMesh)
	{
		BoxMesh->SetRenderCustomDepth(true);
	}
}

void AOrderBox::EndFocus()
{
	if (BoxMesh)
	{
		BoxMesh->SetRenderCustomDepth(false);
	}
}
