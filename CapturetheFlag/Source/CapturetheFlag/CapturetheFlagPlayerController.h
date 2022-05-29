// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CapturetheFlagPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API ACapturetheFlagPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay();
	
};
