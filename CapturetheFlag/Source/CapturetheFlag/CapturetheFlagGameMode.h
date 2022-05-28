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

	void GameModeTimeOutFunc();
};



