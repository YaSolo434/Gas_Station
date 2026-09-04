#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/InteractionInterface.h"

#include "PlayerCharacter.generated.h"

class UItemBase;
class UInventoryComponent;
class ABurgerHUD;
class USpringArmComponent;
class UInputAction;
class UCameraComponent;
struct FInputActionValue;

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

	UPROPERTY(EditDefaultsOnly, Category = "Movement Settings | Crouch")
	FVector CrouchEyeOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Movement Settings | Crouch")
	float MovementSpeedCrouched = 400.0f;

	bool bIsMoving = false;

	// target interactable
	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UInventoryComponent* PlayerInventory;

	float InteractionCheckFrequency;

	float InteractionCheckDistance;

	FTimerHandle TimerHandle_Interaction;

	FInteractionData InteractionData;

	//held item
	UPROPERTY(VisibleAnywhere, Category = "Hold | Component")
	USceneComponent* ItemHoldSocket;

	UPROPERTY(VisibleAnywhere, Category = "Hold | Mesh")
	UStaticMeshComponent* HeldItemMesh;

	UPROPERTY(EditAnywhere, Category = "Hold | Mesh")
	FVector BaseHoldLocation = FVector(30.f, 15.f, -15.f);

	UPROPERTY(EditAnywhere, Category = "Hold | Mesh")
	FRotator BaseHoldRotation = FRotator(30.f, 15.f, -15.f);

	UPROPERTY(EditAnywhere, Category = "Hold | Sway")
	float LocationLagSpeed = 10.f;

	UPROPERTY(EditAnywhere, Category = "Hold|Sway")
	float YawSwayScale = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Hold|Sway")
	float PitchSwayScale = 0.3f;

	UPROPERTY(EditAnywhere, Category = "Hold|Sway")
	float MaxSwayDistance = 5.f;

	FRotator LastCameraRotation;
	FVector CurrentSwayOffset = FVector::ZeroVector;

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

	void StartCrouch();

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	void OnHotbarScroll(const FInputActionValue& Value);
	void OnHotbarSelectSlot(const FInputActionValue& Value);

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

	void DropItem(const FInputActionValue& Value);

	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); }

	FORCEINLINE UInventoryComponent* GetInventory() const { return PlayerInventory; }

	void UpdateHeldItemMesh(const UItemBase* ItemIn) const;

	void RemoveSelectedItem() const;

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult) override;

protected:
	//=================================================================================================
	// PROPERTIES & VARIABLES
	//=================================================================================================
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input | Movement")
	UInputAction* MoveForwardAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input | Movement")
	UInputAction* MoveRightAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input | Movement")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input | Movement")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input | Movement")
	UInputAction* CrouchAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input | Interaction")
	UInputAction* InteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input | Hotbar")
	UInputAction* HotbarScrollAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input | Hotbar")
	UInputAction* HotbarSelectSlotAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input | Hotbar")
	UInputAction* DropItemAction;
};
