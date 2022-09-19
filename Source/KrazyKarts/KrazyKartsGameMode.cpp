// Copyright Epic Games, Inc. All Rights Reserved.

#include "KrazyKartsGameMode.h"
#include "KrazyKartsPawn.h"
#include "KrazyKartsHud.h"

AKrazyKartsGameMode::AKrazyKartsGameMode()
{
	DefaultPawnClass = AKrazyKartsPawn::StaticClass();
	HUDClass = AKrazyKartsHud::StaticClass();
}
