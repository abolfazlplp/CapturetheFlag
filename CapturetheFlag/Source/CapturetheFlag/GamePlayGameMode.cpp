// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayGameMode.h"

void AGamePlayGameMode::PostLogin(APlayerController* NewPlayer) 
{
	Super::PostLogin(NewPlayer);
}

void AGamePlayGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}
