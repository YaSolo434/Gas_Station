// YaSolo


#include "World/RecycleBin.h"

#include "Characters/PlayerCharacter.h"

// Sets default values
ARecycleBin::ARecycleBin()
{
	BinMesh = CreateDefaultSubobject<UStaticMeshComponent>("BinMesh");
	BinMesh->SetSimulatePhysics(false);
	SetRootComponent(BinMesh);
}

// Called when the game starts or when spawned
void ARecycleBin::BeginPlay()
{
	Super::BeginPlay();

	UpdateInteractableData();
}

void ARecycleBin::Interact(APlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		PlayerCharacter->RemoveSelectedItem();
	}
}

void ARecycleBin::BeginFocus()
{
	if (BinMesh)
	{
		BinMesh->SetRenderCustomDepth(true);
	}
}

void ARecycleBin::EndFocus()
{
	if (BinMesh)
	{
		BinMesh->SetRenderCustomDepth(false);
	}
}

void ARecycleBin::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::Device;
	InstanceInteractableData.Action = FText::FromString("Delete Item from");
	InstanceInteractableData.Name = FText::FromString("Inventory");

	InteractableData = InstanceInteractableData;
}
