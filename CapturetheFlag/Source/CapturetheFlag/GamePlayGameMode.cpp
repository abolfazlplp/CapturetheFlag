// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayGameMode.h"
#include "CapturetheFlag\CapturetheFlagHUD.h"

#include "UObject/ConstructorHelpers.h"

AGamePlayGameMode::AGamePlayGameMode()
	:Super()
{
	static ConstructorHelpers::FClassFinder<ACapturetheFlagHUD> HUDClassFinder(TEXT("/Game/HUD/GameplayHUD"));
	HUDClass = HUDClassFinder.Class;
}

void AGamePlayGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FTimerHandle UselessTimer;
	GetWorldTimerManager().SetTimer(UselessTimer, this, &AGamePlayGameMode::GameFinished, 60, false);
}

void AGamePlayGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void AGamePlayGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void AGamePlayGameMode::GameFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Finished!"));

	FTimerHandle UselessTimerHandle;
	GetWorldTimerManager().SetTimer(UselessTimerHandle, this, &AGamePlayGameMode::GameModeTimeOutFunc, 10, false);
}