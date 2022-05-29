// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CapturetheFlagGameState.generated.h"


/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API ACapturetheFlagGameState : public AGameStateBase
{
	GENERATED_BODY()

//Functions
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

//Properties
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Score)
	int TeamBlueScore = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Score)
	int TeamRedScore = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Result)
	FText MatchResult = FText();
};
