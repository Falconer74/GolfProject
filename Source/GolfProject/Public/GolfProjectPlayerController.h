// Golf diploma work. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "GolfProjectPlayerController.generated.h"

class UInputMappingContext;
class UEnhancedInputComponent;
class UInputAction;
class ABallPawn;
class UEnhancedInputLocalPlayerSubsystem;

DEFINE_LOG_CATEGORY_STATIC(GolfPlayerControllerCategory, All, All);

/**
 * 
 */
UCLASS(Abstract)
class GOLFPROJECT_API AGolfProjectPlayerControllerBase : public APlayerController
{
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> LookAroundAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> ChooseDirectionAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> HitAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> ToggleLookAroundAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> ZoomAction = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> MovingBallInputContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> ChoosingDirectionInputContext = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> LookingAroundInputContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> StationaryBallBasicInputContext = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ZoomSpeed = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CameraSensitivity = 1.0f;
	
protected:
	TObjectPtr<class AGolfProjectHUDBase> GolfHUD = nullptr;
	TObjectPtr<class AGolfPlayerState> GolfPlayerState = nullptr;
	
	UFUNCTION()
	FVector      GetDesiredLocation(const float MouseX, const float MouseY) const;
	
	UFUNCTION()
	void         LookAround(const FInputActionValue& InputActionValue);
	
	UFUNCTION()
	void         ChooseHitDirection();
	
	UFUNCTION()
	void         HitBall();
	
	UFUNCTION()
	void         Zoom(const FInputActionValue& InputActionValue);
	
	UFUNCTION()
	virtual void BallMovingContext(ABallPawn* Ball);
	
	UFUNCTION()
	virtual void BallStopMovingContext(ABallPawn* Ball);

	void         ToggleOnLookAround(const FInputActionInstance& InputActionInstance);
	void         ToggleOffLookAround(const FInputActionInstance& InputActionInstance);

	UFUNCTION()
	void            BallHit(ABallPawn* Ball);
	UFUNCTION()
	void            PowerChanged(ABallPawn* Ball, float Power, float MaxPower);
	
	virtual void    OnPossess(APawn* aPawn) override;
	virtual void    OnUnPossess() override;

	UFUNCTION(Client, Reliable)
	void PlayerControllerClientClean();

	UNetDriver* GetNetDriverHelper() const;

public:
	virtual void AcknowledgePossession(APawn* P) override;

private:
	GENERATED_BODY()

	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent = nullptr;

	UPROPERTY()
	ABallPawn* BallPawn = nullptr;

	UPROPERTY()
	bool bLookingAround = false;

	UPROPERTY()
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSubsystem = nullptr;
};
