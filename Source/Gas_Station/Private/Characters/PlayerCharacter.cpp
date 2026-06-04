#include "Characters/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"

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
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

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
	}
}

void APlayerCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart{GetPawnViewLocation()};
	FVector TraceEnd{TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance)};

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0, 0, 2.0f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FHitResult HitResult;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, QueryParams))
	{
		if (HitResult.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
		{
			float Distance = FVector::Distance(TraceStart, HitResult.ImpactPoint);

			if (HitResult.GetActor() != InteractionData.CurrentInteractable && Distance <= InteractionCheckDistance)
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
}

void APlayerCharacter::NoInteractableFound()
{
}

void APlayerCharacter::BeginInteraction()
{
}

void APlayerCharacter::EndInteraction()
{
}

void APlayerCharacter::Interact()
{
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
