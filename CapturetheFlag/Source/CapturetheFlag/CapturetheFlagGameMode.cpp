// Copyright Epic Games, Inc. All Rights Reserved.

#include "CapturetheFlagGameMode.h"
#include "CapturetheFlagHUD.h"
#include "CapturetheFlagCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACapturetheFlagGameMode::ACapturetheFlagGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ACapturetheFlagHUD::StaticClass();
}
