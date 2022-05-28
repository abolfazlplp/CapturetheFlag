// Copyright Epic Games, Inc. All Rights Reserved.

#include "CapturetheFlagGameMode.h"
#include "CapturetheFlagHUD.h"
#include "CapturetheFlagCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet\GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

#include "CapturetheFlagPlayerController.h"

ACapturetheFlagGameMode::ACapturetheFlagGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	PlayerControllerClass = ACapturetheFlagPlayerController::StaticClass();

	// use our custom HUD class
	HUDClass = ACapturetheFlagHUD::StaticClass();
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

void ACapturetheFlagGameMode::Server_RespawnRequest_Implementation(FTransform SpawnTransform, ACapturetheFlagPlayerController* PlayerController)
{
	if (!PlayerController) return;

	auto SpawnedCharacter =GetWorld()->SpawnActor<ACapturetheFlagCharacter>(DefaultPawnClass, SpawnTransform);
	PlayerController->Possess(SpawnedCharacter);
}

void ACapturetheFlagGameMode::AfterSpawnDelegate(ACapturetheFlagCharacter* Player, ACapturetheFlagPlayerController* PlayerController)
{
}
