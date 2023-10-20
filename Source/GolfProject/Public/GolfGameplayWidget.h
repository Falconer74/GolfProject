// Golf diploma work. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GolfGameplayWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class GOLFPROJECT_API UGolfGameplayWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* PowerBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* HitCount;

public:
	void SetPower(float Power, float MaxPower);

	void ResetHitCount();
	void SetHitCount(int Count);
};
