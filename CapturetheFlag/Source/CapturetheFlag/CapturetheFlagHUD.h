// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CapturetheFlagHUD.generated.h"

UCLASS()
class ACapturetheFlagHUD : public AHUD
{
	GENERATED_BODY()

public:
	ACapturetheFlagHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

