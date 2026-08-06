// YaSolo
#include "World/PreparationStation.h"

#include "Characters/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/InventoryComponent.h"
#include "Items/ItemBase.h"
#include "World/CompletedBurger.h"
#include "Data/ItemDataStruct.h"

// Sets default values
APreparationStation::APreparationStation()
{
	WorkSpace = CreateDefaultSubobject<UBoxComponent>(TEXT("WorkSpace"));
	SetRootComponent(WorkSpace);
	WorkSpace->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	WorkSpace->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	WorkSpace->SetVisibility(true);
	WorkSpace->SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void APreparationStation::BeginPlay()
{
	Super::BeginPlay();

	UpdateInteractableData();
}

bool APreparationStation::AddIngredient(const UItemBase* Ingredient)
{
	if (!CanAddIngredient(Ingredient))
	{
		return false;
	}

	UStaticMesh* AssetMesh = Ingredient->AssetData.Mesh;
	if (!AssetMesh)
	{
		return false;
	}

	UStaticMeshComponent* NewMesh = NewObject<UStaticMeshComponent>(this);
	NewMesh->SetStaticMesh(AssetMesh);
	NewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	NewMesh->RegisterComponent();

	//Attach to socket if it's not the first ingredient
	if (IngredientCount == 0)
	{
		NewMesh->SetWorldLocation(CalculateFirstIngredientPosition());
	}
	else
	{
		if (UStaticMeshComponent* LastIngredientMesh = GetLastPlacedMesh())
		{
			NewMesh->AttachToComponent(
				LastIngredientMesh,
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				NextIngredientSocketName);
		}
	}

	AssembledIngredientTypes.Add(Ingredient->FoodType);
	AssembledMeshes.Add(NewMesh);
	IngredientCount++;

	UpdateAssemblyState(Ingredient);

	return true;
}

UStaticMeshComponent* APreparationStation::GetLastPlacedMesh() const
{
	if (AssembledMeshes.Num() > 0)
	{
		return AssembledMeshes.Last();
	}
	return nullptr;
}

bool APreparationStation::CanAddIngredient(const UItemBase* Ingredient) const
{
	if (bIsComplete || !Ingredient)
	{
		return false;
	}
	if (IngredientCount == 0 && Ingredient->FoodType != EFoodType::BreadBottom)
	{
		return false;
	}

	return true;
}

FVector APreparationStation::CalculateFirstIngredientPosition() const
{
	//Get box center and half extends
	const FVector BoxCenter = WorkSpace->GetComponentLocation();
	const FVector BoxExtent = WorkSpace->GetScaledBoxExtent();

	return FVector(BoxCenter.X, BoxCenter.Y, BoxCenter.Z + BoxExtent.Z);
}

void APreparationStation::UpdateAssemblyState(const UItemBase* Ingredient)
{
	if (IsBreadBottom(Ingredient))
	{
		bHasBottomBread = true;
	}
	else if (IsBreadTop(Ingredient))
	{
		bHasTopBread = true;
		bIsComplete = true;
		OnAssemblyComplete();
	}
}

void APreparationStation::OnAssemblyComplete()
{
	// store the old assembled mesh
	TArray<UStaticMesh*> IngredientMeshes;

	for (const UStaticMeshComponent* Mesh : AssembledMeshes)
	{
		if (Mesh && Mesh->GetStaticMesh())
		{
			IngredientMeshes.Add(Mesh->GetStaticMesh());
		}
	}

	//clear the old assembled mesh
	ClearAssembly();

	//Spawn new completed mesh
	const FVector SpawnLocation = CalculateFirstIngredientPosition();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ACompletedBurger* Burger = GetWorld()->SpawnActor<ACompletedBurger>(
		CompletedBurgerClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

	const float BoxCompOffset = Burger->GetComponentsBoundingBox().GetExtent().Z;

	Burger->SetActorLocation(SpawnLocation + FVector(0.f, 0.f, BoxCompOffset));

	if (Burger)
	{
		//setup burger item data for interaction and so on
		if (ItemDataTable && !DesiredItemID.IsNone())
		{
			const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(
				DesiredItemID, DesiredItemID.ToString());

			if (ItemData)
			{
				Burger->ItemReference = NewObject<UItemBase>(Burger);
				Burger->ItemReference->ID = ItemData->ID;
				Burger->ItemReference->ItemType = ItemData->ItemType;
				Burger->ItemReference->FoodType = ItemData->FoodType;
				Burger->ItemReference->DescriptiveText = ItemData->DescriptiveText;
				Burger->ItemReference->AssetData = ItemData->AssetData;
				Burger->ItemReference->IngredientTypes = AssembledIngredientTypes;

				Burger->ItemReference->IngredientMeshes.Empty();
				for (UStaticMesh* Mesh : IngredientMeshes)
				{
					Burger->ItemReference->IngredientMeshes.Add(Mesh);
				}
				Burger->ItemReference->bHasIngredientMeshes = true;

				Burger->UpdateInteractableData();
			}
		}

		//add ingredient meshes to burger
		for (UStaticMesh* Mesh : IngredientMeshes)
		{
			Burger->AddIngredientMesh(Mesh);
		}
	}

	// reset workstation state
	bHasBottomBread = false;
	bHasTopBread = false;
	bIsComplete = false;
	IngredientCount = 0;
	AssembledIngredientTypes.Empty();

	UpdateInteractableData();
}

void APreparationStation::ClearAssembly()
{
	for (UStaticMeshComponent* Mesh : AssembledMeshes)
	{
		if (Mesh)
		{
			Mesh->DestroyComponent();
		}
	}
	AssembledMeshes.Empty();
}

bool APreparationStation::IsBreadBottom(const UItemBase* Ingredient) const
{
	return Ingredient && Ingredient->FoodType == EFoodType::BreadBottom;
}

bool APreparationStation::IsBreadTop(const UItemBase* Ingredient) const
{
	return Ingredient && Ingredient->FoodType == EFoodType::BreadTop;
}

void APreparationStation::Interact(APlayerCharacter* PlayerCharacter)
{
	// if complete do noting player should handle it
	if (bIsComplete)
	{
		return;
	}

	UInventoryComponent* PlayerInventory = PlayerCharacter->GetInventory();

	if (PlayerInventory)
	{
		UItemBase* HeldItem = PlayerInventory->GetSelectedItem();

		if (HeldItem && HeldItem->ItemType == EItemType::Food && HeldItem->FoodType != EFoodType::None)
		{
			if (AddIngredient(HeldItem))
			{
				PlayerInventory->RemoveSelectedItem();
			}
		}
	}
}

void APreparationStation::UpdateInteractableData()
{
	if (bIsComplete)
	{
		InstanceInteractableData.InteractableType = EInteractableType::Pickup;
		InstanceInteractableData.Name = FText::FromString("Completed Burger");
		InstanceInteractableData.Action = FText::FromString("Pick up");
		InstanceInteractableData.InteractionDuration = 0.f;
	}
	else
	{
		InstanceInteractableData.InteractableType = EInteractableType::Device;
		InstanceInteractableData.Name = FText::FromString("Table");
		InstanceInteractableData.Action = FText::FromString("Add Ingredient to");
	}

	InteractableData = InstanceInteractableData;
}

void APreparationStation::BeginFocus()
{
}

void APreparationStation::EndFocus()
{
}
