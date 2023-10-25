// Golf diploma work. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GolfPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitCounterChanged, int, HitCount);

/**
 * 
 */
UCLASS()
class GOLFPROJECT_API AGolfPlayerState : public APlayerState
{
public:
	AGolfPlayerState();

private:
	GENERATED_BODY()

	int PrevHitCount;
	
	UFUNCTION()
	void HitsCountChanged();

public:
	UPROPERTY(ReplicatedUsing=HitsCountChanged, BlueprintReadWrite, Category="BallHit")
	int HitCount = 0;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AddHit();
	int GetHitsCount() const;

	FOnHitCounterChanged OnHitCounterChanged;
};
