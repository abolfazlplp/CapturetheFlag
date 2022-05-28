// Fill out your copyright notice in the Description page of Project Settings.


#include "CapturetheFlagPlayerController.h"
#include "CapturetheFlagGameMode.h"

#include "Kismet/GameplayStatics.h"

void ACapturetheFlagPlayerController::Server_Respawn_Implementation(FTransform SpawnTransform)
{
	auto GameModeBase = UGameplayStatics::GetGameMode(GetWorld());
	if (!GameModeBase) return;

	if (ACapturetheFlagGameMode* GameMode = Cast<ACapturetheFlagGameMode>(GameModeBase))
	{
		GameMode->Server_RespawnRequest(SpawnTransform, this);
	}
}