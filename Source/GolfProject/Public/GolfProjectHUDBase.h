// Golf diploma work. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GolfGameplayWidget.h"
#include "GameFramework/HUD.h"
#include "GolfProjectHUDBase.generated.h"

/**
 * 
 */
UCLASS()
class GOLFPROJECT_API AGolfProjectHUDBase : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGolfGameplayWidget> GameplayWidgetClass;
	
	UPROPERTY()
	UGolfGameplayWidget* GameplayWidget;

	UFUNCTION(BlueprintCallable)
	void ShowGameplayWidget();

	UFUNCTION(BlueprintCallable)
	void HideGameplayWidget();

	UFUNCTION(BlueprintCallable)
	void SetHitCounterValue(int Hits);

	UFUNCTION(BlueprintCallable)
	void SetPowerBarValue(float Power, float MaxPower);
};
