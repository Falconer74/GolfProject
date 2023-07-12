// Golf diploma work. All rights reserved


#include "GolfPlayerState.h"

void AGolfPlayerState::AddHit()
{
	HitCount += 1;
}

int AGolfPlayerState::GetHitsCount()
{
	return HitCount;
}
