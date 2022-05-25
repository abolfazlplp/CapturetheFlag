// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CapturetheFlagGameMode.h"
#include "GamePlayGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API AGamePlayGameMode : public ACapturetheFlagGameMode
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
};
