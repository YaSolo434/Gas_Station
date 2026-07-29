// YaSolo
#include "World/CompletedBurger.h"

#include "Components/BoxComponent.h"
#include "World/PreparationStation.h"

ACompletedBurger::ACompletedBurger()
{
	// Container for ingredient meshes                                                                                                                                                                                                  
	IngredientContainer = CreateDefaultSubobject<UBoxComponent>(TEXT("IngredientContainer"));
	IngredientContainer->SetBoxExtent(FVector(25.0f, 25.0f, 25.0f));
	IngredientContainer->SetCollisionProfileName(TEXT("BlocAllDynamic"));
	SetRootComponent(IngredientContainer);

	PickupMesh->SetupAttachment(IngredientContainer);
	PickupMesh->SetVisibility(false);
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
	NewMesh->RegisterComponent();

	if (IngredientMeshes.Num() == 0)
	{
		// First ingredient attaches directly to the container
		NewMesh->AttachToComponent(
			IngredientContainer,
			FAttachmentTransformRules::KeepRelativeTransform);
		NewMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -10.f));
	}
	else
	{
		// Subsequent ingredients snap to the previous mesh's socket
		UStaticMeshComponent* LastMesh = IngredientMeshes.Last();

		NewMesh->AttachToComponent(
			LastMesh,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			NextIngredientSocketName);
	}

	IngredientMeshes.Add(NewMesh);
}

void ACompletedBurger::BeginFocus()
{
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
	for (UStaticMeshComponent* Mesh : IngredientMeshes)
	{
		if (Mesh)
		{
			Mesh->SetRenderCustomDepth(false);
		}
	}
}
