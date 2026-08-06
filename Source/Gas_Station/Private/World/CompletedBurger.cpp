// YaSolo
#include "World/CompletedBurger.h"

#include "Components/BoxComponent.h"
#include "QuestSystem/OrderSubSystem.h"
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
		NewMesh->SetRelativeLocation(FVector(0.f, 0.f, -this->GetComponentsBoundingBox().GetExtent().Z));
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

	//Scale up the burger container extend as the burger grows
	if (IngredientMeshes.Num() > 5 && IngredientMeshes.Num() % 5 == 0)
	{
		IngredientContainer->SetBoxExtent(IngredientContainer->GetUnscaledBoxExtent() + FVector(0.f, 0.f, 10.f));
	}

	IngredientMeshes.Add(NewMesh);
}

void ACompletedBurger::ServeToCustomer(FGuid OrderID)
{
	if (ItemReference)
	{
		if (UOrderSubSystem* OrderSubSystem = GetWorld()->GetSubsystem<UOrderSubSystem>())
		{
			bool bWasCorrect = OrderSubSystem->SubmitBurger(OrderID, ItemReference);

			//TODO: the removal of the item
		}
	}
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
