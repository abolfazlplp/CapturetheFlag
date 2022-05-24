// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureTheFlagGameInstance.h"
#include "SubSystemManager.h"

UCaptureTheFlagGameInstance::UCaptureTheFlagGameInstance(const FObjectInitializer& ObjectInitializer)
{}

void UCaptureTheFlagGameInstance::Init() 
{
	SubsystemManager = NewObject<USubSystemManager>(this, "Subsystem Manager");

	if (SubsystemManager != nullptr)
		SubsystemManager->Initialize();

}
