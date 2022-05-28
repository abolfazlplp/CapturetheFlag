// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CapturetheFlagGameMode.generated.h"

UCLASS(minimalapi)
class ACapturetheFlagGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACapturetheFlagGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION(Server, Reliable)
	void Server_RespawnRequest(FTransform SpawnTransform, class ACapturetheFlagPlayerController* PlayerController);
	void Server_RespawnRequest_Implementation(FTransform SpawnTransform,class ACapturetheFlagPlayerController* PlayerController);

	UFUNCTION()
	void AfterSpawnDelegate(ACapturetheFlagCharacter* Player, class ACapturetheFlagPlayerController* PlayerController);
};



