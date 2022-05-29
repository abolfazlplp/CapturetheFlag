// Fill out your copyright notice in the Description page of Project Settings.


#include "CapturetheFlagGameState.h"

#include "Net\UnrealNetwork.h"

void ACapturetheFlagGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACapturetheFlagGameState, TeamBlueScore);
	DOREPLIFETIME(ACapturetheFlagGameState, TeamRedScore);
	DOREPLIFETIME(ACapturetheFlagGameState, MatchResult);
}