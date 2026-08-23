#include "Characters/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/InteractionInterface.h"
#include "DrawDebugHelpers.h"
#include "UserInterface/BurgerHUD.h"
#include "Components/InventoryComponent.h"
#include "Items/ItemBase.h"
#include "QuestSystem/OrderSubSystem.h"
#include "World/Pickup.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create CameraBoom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoom"));
	CameraBoom->SetupAttachment(GetCapsuleComponent());
	CameraBoom->TargetArmLength = 0.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f));

	//Create Camera
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FirstPersonCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FirstPersonCamera->bUsePawnControlRotation = false;
	FirstPersonCamera->FieldOfView = CameraFOV;

	//Held item
	ItemHoldSocket = CreateDefaultSubobject<USceneComponent>(TEXT("ItemHoldSocket"));
	ItemHoldSocket->SetupAttachment(FirstPersonCamera);
	ItemHoldSocket->SetRelativeLocation(BaseHoldLocation);
	ItemHoldSocket->SetRelativeRotation(BaseHoldRotation);

	HeldItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeldItemMesh"));
	HeldItemMesh->SetupAttachment(ItemHoldSocket);
	HeldItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HeldItemMesh->SetCastShadow(false);

	//Set player movement settings
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->JumpZVelocity = 800.f;
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	InteractionCheckFrequency = 0.1f;
	InteractionCheckDistance = 225.0f;

	//inventory
	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
	PlayerInventory->SetSlotsCapacity(8);~
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	HUD = Cast<ABurgerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	//Limit camera
	if (APlayerCameraManager* const CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager)
	{
		CameraManager->ViewPitchMin = -90.f;
	}

	//Add input mapping context
	if (const APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->TimeSince(InteractionData.LastInteractionTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}

	const FRotator CurrentCamRot = FirstPersonCamera->GetComponentRotation();
	const FRotator DeltaRot = (CurrentCamRot - LastCameraRotation).GetNormalized();

	LastCameraRotation = CurrentCamRot;

	const FVector TargetSwayLoc{
		0.f,
		FMath::Clamp(-DeltaRot.Yaw * YawSwayScale, -MaxSwayDistance, MaxSwayDistance),
		FMath::Clamp(DeltaRot.Pitch * PitchSwayScale, -MaxSwayDistance, MaxSwayDistance)
	};

	CurrentSwayOffset = FMath::VInterpTo(CurrentSwayOffset, TargetSwayLoc, DeltaTime, LocationLagSpeed);

	ItemHoldSocket->SetRelativeLocation(BaseHoldLocation + CurrentSwayOffset);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this,
		                                   &APlayerCharacter::MoveForward);
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Completed, this,
		                                   &APlayerCharacter::StopMoveForward);

		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this,
		                                   &APlayerCharacter::MoveRight);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Completed, this,
		                                   &APlayerCharacter::StopMoveRight);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,
		                                   &APlayerCharacter::Look);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this,
		                                   &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this,
		                                   &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this,
		                                   &APlayerCharacter::BeginInteract);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this,
		                                   &APlayerCharacter::EndInteract);

		EnhancedInputComponent->BindAction(HotbarScrollAction, ETriggerEvent::Triggered, this,
		                                   &APlayerCharacter::OnHotbarScroll);

		EnhancedInputComponent->BindAction(HotbarSelectSlotAction, ETriggerEvent::Triggered, this,
		                                   &APlayerCharacter::OnHotbarSelectSlot);

		EnhancedInputComponent->BindAction(DropItemAction, ETriggerEvent::Triggered, this,
		                                   &APlayerCharacter::DropItem);
	}
}

void APlayerCharacter::DropItem(const FInputActionValue& Value)
{
	if (!PlayerInventory->GetSelectedItem())
	{
		return;
	}
	if (PlayerInventory->GetSelectedItem()->bHasIngredientMeshes)
	{
		return;
	}

	UOrderSubSystem* OrderSubSystem = GetWorld()->GetSubsystem<UOrderSubSystem>();

	OrderSubSystem->GenerateRandomOrder();

	UItemBase* DroppedItem = PlayerInventory->RemoveSelectedItem();

	if (DroppedItem)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const FVector SpawnLocation{
			GetActorLocation() + FVector{0.f, 0.f, BaseEyeHeight - 15} + (GetActorForwardVector() * 30.0f)
		};
		const FTransform SpawnTransform{GetActorRotation(), SpawnLocation};

		APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), SpawnTransform, SpawnParams);

		//set physics true after throwing it
		Pickup->GetStaticMeshComponent()->SetSimulatePhysics(true);

		Pickup->InitializeDrop(DroppedItem);

		//for the impulse
		FVector ThrowDirection = GetActorForwardVector() + FVector(0.f, 0.f, 0.4f);
		ThrowDirection.Normalize();

		constexpr float ThrowStrength = 1000.f;

		Pickup->GetStaticMeshComponent()->AddImpulse(ThrowDirection * ThrowStrength, NAME_None, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Dropped Item Not Found"));
	}
}

void APlayerCharacter::RemoveSelectedItem() const
{
	if (PlayerInventory->RemoveSelectedItem())
	{
		//delete the old completed burger mesh from hand(if theres any)
		TArray<USceneComponent*> AllDescendants;
		ItemHoldSocket->GetChildrenComponents(true, AllDescendants);

		for (const auto Child : AllDescendants)
		{
			if (Child != HeldItemMesh)
			{
				Child->DestroyComponent(true);
			}
		}
	}
}

void APlayerCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionTime = GetWorld()->GetTimeSeconds();

	const FVector TraceStart{GetPawnViewLocation()};
	const FVector TraceEnd{TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance)};

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FHitResult HitResult;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		if (HitResult.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
		{
			if (HitResult.GetActor() != InteractionData.CurrentInteractable)
			{
				FoundInteractable(HitResult.GetActor());
				return;
			}

			if (HitResult.GetActor() == InteractionData.CurrentInteractable)
			{
				return;
			}
		}
	}
	NoInteractableFound();
}

void APlayerCharacter::FoundInteractable(AActor* NewInteractable)
{
	//checks if new interactable found if it founds it ends interaction with the previous one
	if (IsInteracting())
	{
		EndInteract();
	}

	// checks if we have Current interactable
	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	//assign the new interaction target
	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);

	TargetInteractable->BeginFocus();
}

void APlayerCharacter::NoInteractableFound()
{
	if (IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if (InteractionData.CurrentInteractable)
	{
		if (TargetInteractable.GetObject())
		{
			TargetInteractable->EndFocus();
		}
	}

	HUD->HideInteractionWidget();

	TargetInteractable = nullptr;
	InteractionData.CurrentInteractable = nullptr;
}

void APlayerCharacter::BeginInteract()
{
	// verify nothing has changed with the interactable state since beginning interaction
	PerformInteractionCheck();

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();

			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction,
				                                this,
				                                &APlayerCharacter::Interact,
				                                TargetInteractable->InteractableData.InteractionDuration,
				                                false);
			}
		}
	}
}

void APlayerCharacter::EndInteract()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

void APlayerCharacter::Interact()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(this);
	}
}

void APlayerCharacter::UpdateHeldItemMesh(const UItemBase* ItemIn) const
{
	//delete the old completed burger mesh from hand(if theres any)
	TArray<USceneComponent*> AllDescendants;
	ItemHoldSocket->GetChildrenComponents(true, AllDescendants);

	for (const auto Child : AllDescendants)
	{
		if (Child != HeldItemMesh)
		{
			Child->DestroyComponent(true);
		}
	}

	HeldItemMesh->SetStaticMesh(nullptr);

	if (!ItemIn)
	{
		return;
	}

	if (ItemIn->bHasIngredientMeshes)
	{
		UStaticMeshComponent* PreviousSM = nullptr;
		for (int32 i = 0; i < ItemIn->IngredientMeshes.Num(); i++)
		{
			UStaticMeshComponent* CurrentSM = NewObject<UStaticMeshComponent>(ItemHoldSocket);
			CurrentSM->SetStaticMesh(ItemIn->IngredientMeshes[i]);
			CurrentSM->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			CurrentSM->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
			CurrentSM->RegisterComponent();

			if (i == 0)
			{
				CurrentSM->AttachToComponent(ItemHoldSocket, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			}
			else
			{
				CurrentSM->AttachToComponent(PreviousSM, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				                             "NextIngredient_Socket");
			}
			PreviousSM = CurrentSM;
		}
	}
	else
	{
		HeldItemMesh->SetStaticMesh(ItemIn->AssetData.Mesh);
		HeldItemMesh->SetRelativeLocation(FVector::ZeroVector);
		HeldItemMesh->SetRelativeRotation(FRotator::ZeroRotator);
	}
}


void APlayerCharacter::MoveForward(const FInputActionValue& Value)
{
	bIsMoving = true;

	float AxisValue = Value.Get<float>();
	if (Controller && AxisValue != 0)
	{
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, AxisValue);
	}
}

void APlayerCharacter::StopMoveForward(const FInputActionValue& Value)
{
	if (bIsMoving)
	{
		bIsMoving = false;
	}
}

void APlayerCharacter::MoveRight(const FInputActionValue& Value)
{
	bIsMoving = true;

	float AxisValue = Value.Get<float>();

	if (Controller && AxisValue != 0)
	{
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector CameraRotationVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(CameraRotationVector, AxisValue);
	}
}

void APlayerCharacter::StopMoveRight(const FInputActionValue& Value)
{
	if (bIsMoving)
	{
		bIsMoving = false;
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookInput = Value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void APlayerCharacter::OnHotbarScroll(const FInputActionValue& Value)
{
	const float AxisValue = Value.Get<float>();
	PlayerInventory->ScrollHotbar(AxisValue > 0.f ? 1 : -1);
}

void APlayerCharacter::OnHotbarSelectSlot(const FInputActionValue& Value)
{
	const int32 SlotIndex = FMath::RoundToInt(Value.Get<float>()) - 1;
	PlayerInventory->SelectSlot(SlotIndex);
}
