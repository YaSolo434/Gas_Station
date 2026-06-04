#include "GameModes/GasStationGameBase.h"

#include "Characters/PlayerCharacter.h"

AGasStationGameBase::AGasStationGameBase()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
}
