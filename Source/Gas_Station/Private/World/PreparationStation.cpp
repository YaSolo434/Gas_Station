// YaSolo
#include "World/PreparationStation.h"

#include "Components/BoxComponent.h"
#include "Items/ItemBase.h"

// Sets default values
APreparationStation::APreparationStation()
{
	WorkSpace = CreateDefaultSubobject<UBoxComponent>(TEXT("WorkSpace"));
	SetRootComponent(WorkSpace);
	WorkSpace->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	WorkSpace->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	WorkSpace->SetVisibility(false);
	WorkSpace->SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void APreparationStation::BeginPlay()
{
	Super::BeginPlay();
}

void APreparationStation::Interact(APlayerCharacter* PlayerCharacter)
{
	// ?
}

bool APreparationStation::AddIngredient(UItemBase* Ingredient)
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

	//Attach to socket if its not the first ingrident
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
				FAttachmentTransformRules::SnapToTargetIncludingScale,
				NextIngredientSocketName);
		}
	}

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
	if (Ingredient->ItemType != EItemType::Food)
	{
		return false;
	}

	return true;
}

void APreparationStation::PickupCompletedBurger(APlayerCharacter* PlayerCharacter)
{
	if (!bIsComplete || !PlayerCharacter)
	{
		return;
	}

	ClearAssembly();

	// TODO: Spawn pickup with CompletedBurger item
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
	}
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
	IngredientCount = 0;
	bIsComplete = false;
	bHasBottomBread = false;
	bHasTopBread = false;
}

bool APreparationStation::IsBreadBottom(const UItemBase* Ingredient) const
{
	return Ingredient && Ingredient->FoodType == EFoodType::BreadBottom;
}

bool APreparationStation::IsBreadTop(const UItemBase* Ingredient) const
{
	return Ingredient && Ingredient->FoodType == EFoodType::BreadTop;
}
