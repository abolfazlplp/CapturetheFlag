// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CapturetheFlagGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API ALobbyGameMode : public ACapturetheFlagGameMode
{
	GENERATED_BODY()

protected:
		virtual void PostLogin(APlayerController* NewPlayer) override;
		virtual void Logout(AController* Exiting) override;

private:
	uint32 PlayersNum = 0;
	FTimerHandle LobbyTimeOut;

	void LobbyTimeOutFunc();
};
