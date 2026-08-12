#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GasStationGameBase.generated.h"

UCLASS()
class GAS_STATION_API AGasStationGameBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGasStationGameBase();

	UPROPERTY(EditAnywhere, Category = "Orders")
	UDataTable* ItemTable;

protected:
	virtual void BeginPlay() override;
};
