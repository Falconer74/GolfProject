// Copyright Epic Games, Inc. All Rights Reserved.


#include "GolfProjectGameModeBase.h"
#include "BallPawn.h"
#include "GolfPlayerState.h"

AGolfProjectGameModeBase::AGolfProjectGameModeBase()
{
	DefaultPawnClass = ABallPawn::StaticClass();
	PlayerStateClass = AGolfPlayerState::StaticClass();
}

void AGolfProjectGameModeBase::StartPlay()
{
	Super::StartPlay();
}

void AGolfProjectGameModeBase::Goal(ABallPawn* Ball, AGolfProjectPlayerController* Controller)
{
	UE_LOG(GameModeBaseCategory, Display, TEXT("GOAL!!!"));
}
