// YaSolo
#include "World/CompletedBurger.h"

ACompletedBurger::ACompletedBurger()
{
	IngredientContainer = CreateDefaultSubobject<USceneComponent>("IngredientContainer");
	IngredientContainer->SetupAttachment(RootComponent);
}

void ACompletedBurger::AddIngredientMesh(UStaticMesh* Mesh)
{
	if (!Mesh)
	{
		return;
	}

	UStaticMeshComponent* NewMesh = NewObject<UStaticMeshComponent>(this);
	NewMesh->SetStaticMesh(Mesh);
	NewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	NewMesh->SetupAttachment(IngredientContainer);
	NewMesh->RegisterComponent();

	IngredientMeshes.Add(NewMesh);

	// stack ingredient meshes vertically
	float HeightOffset = 0.f;
	for (const UStaticMeshComponent* ExistingMesh : IngredientMeshes)
	{
		if (ExistingMesh && ExistingMesh != NewMesh)
		{
			FBoxSphereBounds Bounds = ExistingMesh->GetStaticMesh()->GetBounds();
			HeightOffset += Bounds.GetBox().GetSize().Z;
		}
	}

	NewMesh->SetRelativeLocation(FVector(0.f, 0.f, HeightOffset));
}

void ACompletedBurger::BeginFocus()
{
	Super::BeginFocus();

	for (UStaticMeshComponent* Mesh : IngredientMeshes)
	{
		if (Mesh)
		{
			Mesh->SetRenderCustomDepth(true);
		}
	}
}

void ACompletedBurger::EndFocus()
{
	Super::EndFocus();

	for (UStaticMeshComponent* Mesh : IngredientMeshes)
	{
		if (Mesh)
		{
			Mesh->SetRenderCustomDepth(false);
		}
	}
}
