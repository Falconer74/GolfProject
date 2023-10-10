// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "BallPawn.h"
#include "GolfProjectPlayerController.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GolfProjectGameModeBase.generated.h"

DEFINE_LOG_CATEGORY_STATIC(GameModeBaseCategory, All, All);

/**
 * 
 */
UCLASS()
class GOLFPROJECT_API AGolfProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGolfProjectGameModeBase();

	virtual void StartPlay() override;

	UFUNCTION()
	void Goal(ABallPawn* Ball, AGolfProjectPlayerControllerBase* Controller);
};
