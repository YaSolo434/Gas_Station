#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataStruct.generated.h"

USTRUCT()
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;
};

UENUM()
enum class EItemType : uint8
{
	Food UMETA(DisplayName="Food"),
	Equipment UMETA(DisplayName="Equipment"),
};

UENUM()
enum class EFoodType : uint8
{
	None UMETA(DisplayName = "None"),
	BreadBottom UMETA(DisplayName = "Bread Bottom"),
	BreadTop UMETA(DisplayName = "Bread Top"),
	Patty UMETA(DisplayName = "Patty"),
	Cheese UMETA(DisplayName = "Cheese"),
	Tomato UMETA(DisplayName = "Tomato"),
	Lettuce UMETA(DisplayName = "Lettuce"),
	Onion UMETA(DisplayName = "Onion"),
	Pickle UMETA(DisplayName = "Pickle"),
	CompletedBurger UMETA(DisplayName = "Completed Burger"),
};

USTRUCT()
struct FItemDescriptiveTextData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FText DisplayName;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	FText UsageInfo;

	UPROPERTY(EditAnywhere)
	FText InteractionText;
};

USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FItemAssetData AssetData;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "ItemData", meta = (EditCondition = "ItemType == EItemType::Food", EditConditionHides))
	EFoodType FoodType;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FItemDescriptiveTextData DescriptiveText;
};
