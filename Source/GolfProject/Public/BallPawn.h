// Golf diploma work. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BallPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UArrowComponent;

DEFINE_LOG_CATEGORY_STATIC(BallPawnCategory, All, All);

UCLASS()
class GOLFPROJECT_API ABallPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABallPawn();

protected:
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CameraController")
	float ZoomSpeed = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CameraController")
	bool bCameraMoving = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BallHit")
	float MaxVectorLength = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BallHit")
	float HitForce = 50000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerInput")
	bool bBallMoving = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UGolfGameplayWidget> GameplayWidgetClass;

	UPROPERTY()
	class UGolfGameplayWidget* GameplayWidget;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	FVector GetDesiredLocation() const;
	inline bool IsBallMoving() const;
	
	void Hit();
	void HitPreview() const;
	
	void StartMovingCamera();
	void StopMovingCamera();

	void PitchCamera(const float Amount);
	void YawCamera(const float Amount);

	void Zoom(const float X);
};
