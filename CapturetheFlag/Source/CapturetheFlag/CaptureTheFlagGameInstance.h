// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CaptureTheFlagGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API UCaptureTheFlagGameInstance : public UGameInstance
{
	GENERATED_BODY()

//************************************* Constructor ***********************************************
public:
	UCaptureTheFlagGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;

//************************************* Public Properties ***********************************************
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SubSystem)
	class USubSystemManager* SubsystemManager;
};
