#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/InteractionInterface.h"

#include "PlayerCharacter.generated.h"

class ABurgerHUD;

USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

	FInteractionData() :
		CurrentInteractable(nullptr), LastInteractionTime(0.0f)
	{
	};

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionTime;
};

class USpringArmComponent;
class UInputAction;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class GAS_STATION_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	//=================================================================================================
	// PROPERTIES & VARIABLES
	//=================================================================================================
	UPROPERTY()
	ABurgerHUD* HUD;

	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* FirstPersonCamera;

	UPROPERTY(EditAnywhere, Category = "Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, Category = "Camera | Properties")
	float CameraFOV = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Camera | Properties")
	float CameraSensitivity = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	float MovementSpeed = 600.0f;

	bool bIsMoving = false;

	// target interactable
	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	float InteractionCheckFrequency;

	float InteractionCheckDistance;

	FTimerHandle TimerHandle_Interaction;

	FInteractionData InteractionData;

	//=================================================================================================
	// FUNCTIONS
	//=================================================================================================
	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();

	//Input functions
	void MoveForward(const FInputActionValue& Value);
	void StopMoveForward(const FInputActionValue& Value);

	void MoveRight(const FInputActionValue& Value);
	void StopMoveRight(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

public:
	//=================================================================================================
	// PROPERTIES & VARIABLES
	//=================================================================================================
	APlayerCharacter();

	//=================================================================================================
	// FUNCTIONS
	//=================================================================================================
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); }

protected:
	//=================================================================================================
	// PROPERTIES & VARIABLES
	//=================================================================================================
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveForwardAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveRightAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractAction;
};
