// Golf diploma work. All rights reserved


#include "GolfGameplayWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UGolfGameplayWidget::SetPower(float Power, float MaxPower)
{
	if(PowerBar)
	{
		PowerBar->SetPercent(Power / MaxPower);
	}
}

void UGolfGameplayWidget::ResetHitCount()
{
	if(HitCount)
	{
		HitCount->SetText(FText::AsNumber(0));
	}
}

void UGolfGameplayWidget::SetHitCount(int Count)
{
	if(HitCount)
	{
		HitCount->SetText((FText::AsNumber(Count)));
	}
}
