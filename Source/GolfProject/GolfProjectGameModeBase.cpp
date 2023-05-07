// Copyright Epic Games, Inc. All Rights Reserved.


#include "GolfProjectGameModeBase.h"
#include "BallPawn.h"

AGolfProjectGameModeBase::AGolfProjectGameModeBase()
{
	DefaultPawnClass = ABallPawn::StaticClass();
}
