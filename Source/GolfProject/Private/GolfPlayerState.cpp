// Golf diploma work. All rights reserved


#include "GolfPlayerState.h"

#include "Net/UnrealNetwork.h"

AGolfPlayerState::AGolfPlayerState()
{
	bReplicates = true;

	PrevHitCount = HitCount;
}

void AGolfPlayerState::HitsCountChanged()
{
	// For hit counter prediction, to make counter more responsive.
	if(PrevHitCount != HitCount)
	{
		PrevHitCount = HitCount;

		OnHitCounterChanged.Broadcast(HitCount);
	}
}

void AGolfPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGolfPlayerState, HitCount);
}

void AGolfPlayerState::AddHit()
{
	HitCount += 1;

	OnHitCounterChanged.Broadcast(HitCount);
}

int AGolfPlayerState::GetHitsCount() const
{
	return HitCount;
}
