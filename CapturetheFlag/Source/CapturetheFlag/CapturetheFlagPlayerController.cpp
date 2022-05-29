// Fill out your copyright notice in the Description page of Project Settings.

#include "CapturetheFlagPlayerController.h"

void ACapturetheFlagPlayerController::BeginPlay() 
{
    Super::BeginPlay();

    FInputModeGameOnly InputMode;
    SetInputMode(InputMode);
}