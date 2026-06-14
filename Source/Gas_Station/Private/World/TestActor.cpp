// YaSolo


#include "World/TestActor.h"

// Sets default values
ATestActor::ATestActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();

	InteractableData = InstanceInteractableData;
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATestActor::BeginFocus()
{
	if (Mesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Calling begin focus on interface"));
		Mesh->SetRenderCustomDepth(true);
	}
}

void ATestActor::EndFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
	}
}

void ATestActor::BeginInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling begin interact override on interface"));
}

void ATestActor::EndInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling end interact override on interface"));
}

void ATestActor::Interact(APlayerCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("Calling interact override on interface"));
}
