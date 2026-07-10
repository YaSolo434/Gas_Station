// YaSolo

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class ABurgerHUD;
class UItemBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdated, int32, NewSlot);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectedSlotChanged, int32, NewSlot);

UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	IAR_NoItemAdded UMETA(DisplayName = "No item added"),
	IAR_AllItemAdded UMETA(DisplayName = "All of item added")
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	FItemAddResult() : OperationResult(EItemAddResult::IAR_NoItemAdded),
	                   ResultMessage(FText::GetEmpty())
	{
	};

	// Enum representing the end state of an add item operation
	UPROPERTY(BlueprintReadOnly, Category = "Item add result")
	EItemAddResult OperationResult;

	// Informational message that can be passed with the result
	UPROPERTY(BlueprintReadOnly, Category = "Item add result")
	FText ResultMessage;

	static FItemAddResult AddedNone(const FText& ErrorText)
	{
		FItemAddResult AddNoneResult;
		AddNoneResult.OperationResult = EItemAddResult::IAR_NoItemAdded;
		AddNoneResult.ResultMessage = ErrorText;

		return AddNoneResult;
	};

	static FItemAddResult AddedItem(const FText& MessageText)
	{
		FItemAddResult AddResult;
		AddResult.OperationResult = EItemAddResult::IAR_AllItemAdded;
		AddResult.ResultMessage = MessageText;

		return AddResult;
	};
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAS_STATION_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	//=================================================================================================
	// PROPERTIES & VARIABLES
	//=================================================================================================	
	FOnInventoryUpdated OnInventoryUpdated;
	FOnSelectedSlotChanged OnSelectedSlotChanged;

	UPROPERTY(VisibleAnywhere, Category = "Hotbar")
	int32 SelectedSlot = 0;

	UFUNCTION()
	void SelectSlot(const int32 Index);

	UFUNCTION()
	void ScrollHotbar(const int32 Direction);

	FORCEINLINE UItemBase* GetSelectedItem() const { return InventoryContents[SelectedSlot]; }


	//=================================================================================================
	// FUNCTIONS
	//=================================================================================================
	UInventoryComponent();
	virtual void BeginPlay() override;

	UFUNCTION(Category = "Inventory")
	FItemAddResult HandleAddItem(UItemBase* ItemIn);

	UFUNCTION(Category = "Inventory")
	UItemBase* RemoveSelectedItem();

	//getters
	UFUNCTION(Category = "Inventory")
	FORCEINLINE int32 GetSlotsCapacity() const
	{
		return InventorySlotsCapacity;
	};
	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<UItemBase*> GetInventoryContents() const
	{
		return InventoryContents;
	};

	//setters
	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetSlotsCapacity(const int32 NewSlotsCapacity)
	{
		InventorySlotsCapacity = NewSlotsCapacity;
	};

protected:
	//=================================================================================================
	// PROPERTIES & VARIABLES
	//=================================================================================================
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 InventorySlotsCapacity = 9;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UItemBase>> InventoryContents;

	UPROPERTY()
	ABurgerHUD* HUD;

	UPROPERTY(EditAnywhere, Category = "HighlightWidget")
	float HighlightWidgetDurationTime = 2.0f;

	FTimerHandle TimerHandle_HighlightWidget;

	//=================================================================================================
	// FUNCTIONS
	//=================================================================================================
	UFUNCTION(Category = "Inventory")
	void AddNewItem(UItemBase* Item, int32 SlotIndex);

	int32 WrapIndex(const int32 Index) const;
};
