#include "GameModes/GasStationGameBase.h"

#include "Characters/PlayerCharacter.h"
#include "QuestSystem/OrderSubSystem.h"

AGasStationGameBase::AGasStationGameBase()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
}

void AGasStationGameBase::BeginPlay()
{
	Super::BeginPlay();

	if (UOrderSubSystem* OrderSubSystem = GetWorld()->GetSubsystem<UOrderSubSystem>())
	{
		OrderSubSystem->SetItemTable(ItemTable);
	}
}
