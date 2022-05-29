// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "CapturetheFlag\CapturetheFlagCharacter.h"
#include "CapturetheFlag\CaptureTheFlagGameInstance.h"
#include "SubSystemManager.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (PlayersNum == 0)
		GetWorldTimerManager().SetTimer(LobbyTimeOut, this, &ALobbyGameMode::GameModeTimeOutFunc, 180, false);

	++PlayersNum;

	/*if (ACapturetheFlagCharacter* Player = Cast<ACapturetheFlagCharacter>(NewPlayer->GetPawn()))
	{
		Player->PlayerTeam = PlayersNum % 2 == 0 ? ETeam::Red : ETeam::Blue;
	}*/

	if (PlayersNum >= 2)
	{
		UWorld* World = GetWorld();

		if (!ensure(World != nullptr)) return;

		GetWorldTimerManager().ClearTimer(LobbyTimeOut);
		
		FString InURL = "/Game/Maps/GamePlay?listen";
		bUseSeamlessTravel = true;
		World->ServerTravel(InURL);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--PlayersNum;
}