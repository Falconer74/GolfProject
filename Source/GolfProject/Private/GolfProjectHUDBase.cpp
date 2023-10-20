// Golf diploma work. All rights reserved


#include "GolfProjectHUDBase.h"

#include "GolfProjectPlayerController.h"
#include "Blueprint/UserWidget.h"

void AGolfProjectHUDBase::ShowGameplayWidget()
{
	AGolfProjectPlayerControllerBase* GolfController = Cast<AGolfProjectPlayerControllerBase>(GetOwningPlayerController());
	checkf(GolfController, TEXT("GolfController in AGolfProjectHUDBase::ShowGameplayWidget() is nullptr"));
	
	if(GameplayWidgetClass)
	{
		GameplayWidget = CreateWidget<UGolfGameplayWidget>(GolfController, GameplayWidgetClass);
		checkf(GameplayWidget, TEXT("GameplayWidget in AGolfProjectHUDBase::ShowGameplayWidget() is nullptr"));
		GameplayWidget->AddToPlayerScreen();
	}
}

void AGolfProjectHUDBase::HideGameplayWidget()
{
	if(GameplayWidget)
	{
		GameplayWidget->RemoveFromParent();
		GameplayWidget = nullptr;
	}
}

void AGolfProjectHUDBase::SetHitCounterValue(int Hits)
{
	GameplayWidget->SetHitCount(Hits);
}

void AGolfProjectHUDBase::SetPowerBarValue(float Power, float MaxPower)
{
	GameplayWidget->SetPower(Power, MaxPower);
}
