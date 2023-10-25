// Golf diploma work. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BallPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UArrowComponent;

DEFINE_LOG_CATEGORY_STATIC(BallPawnCategory, All, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBallStatusChanged, ABallPawn*, Ball);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPowerChanged, ABallPawn*, Ball, float, Power, float, MaxPower);

UCLASS()
class GOLFPROJECT_API ABallPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABallPawn();

	FOnBallStatusChanged OnBallHit;
	FOnBallStatusChanged OnBallStartMoving;
	FOnBallStatusChanged OnBallStopMoving;
	FOnPowerChanged OnPowerChanged;

protected:
	// TODO
	// Change static components to dynamic components in order to disable it on server side
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BallHit")
	float MaxVectorLength = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BallHit")
	float HitForce = 50000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerInput")
	bool bBallMoving = false;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	virtual void ChangeArrowVisibility(ABallPawn* Ball);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Server, Reliable)
	void Hit(const FVector& DesiredLocation);
	void HitPreview(const FVector& DesiredLocation);
	void Zoom(const float X);

	inline bool IsBallMoving() const;

	UFUNCTION()
	virtual void SetArrowVisibility(bool bVisible);
};
