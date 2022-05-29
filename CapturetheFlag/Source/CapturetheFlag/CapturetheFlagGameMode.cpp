// Copyright Epic Games, Inc. All Rights Reserved.

#include "CapturetheFlagGameMode.h"
#include "CapturetheFlagHUD.h"
#include "CapturetheFlagCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet\GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

#include "CapturetheFlag\CaptureTheFlagGameInstance.h"
#include "SubSystemManager.h"
#include "CapturetheFlagGameState.h"

ACapturetheFlagGameMode::ACapturetheFlagGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ACapturetheFlagHUD::StaticClass();
	GameStateClass = ACapturetheFlagGameState::StaticClass();
}

void ACapturetheFlagGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		APlayerStart* PlayerStart = Cast<APlayerStart>(FoundActor);

		if (PlayerStart->PlayerStartTag !=(TEXT("Taken")))
		{
			NewPlayer->GetPawn()->SetActorTransform(PlayerStart->GetActorTransform());
			PlayerStart->PlayerStartTag = (TEXT("Taken"));
		}
	}
}

void ACapturetheFlagGameMode::GameModeTimeOutFunc()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	if (UCaptureTheFlagGameInstance* GameInstance = GetGameInstance<UCaptureTheFlagGameInstance>())
	{
		GameInstance->SubsystemManager->DestroySession();
	}

	FString InURL = "/Game/Maps/MainMenu";
	bUseSeamlessTravel = false;
	World->ServerTravel(InURL);
}
