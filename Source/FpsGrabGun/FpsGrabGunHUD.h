// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FpsGrabGunHUD.generated.h"

UCLASS()
class AFpsGrabGunHUD : public AHUD
{
	GENERATED_BODY()

public:
	AFpsGrabGunHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

