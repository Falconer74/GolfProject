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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	float MousePosY;
	float MousePosX;

	void Hit();
	void StartMovingCamera();
	void StopMovingCamera();

	void PitchCamera(float Amount);
	void YawCamera(float Amount);

	void Zoom(float X);
};
