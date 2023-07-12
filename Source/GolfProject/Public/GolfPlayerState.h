// Golf diploma work. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GolfPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GOLFPROJECT_API AGolfPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BallHit")
	int HitCount = 0;

	void AddHit();
	int GetHitsCount();
};
