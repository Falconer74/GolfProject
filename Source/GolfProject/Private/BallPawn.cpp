// Golf diploma work. All rights reserved


#include "BallPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "GolfProjectPlayerController.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "DrawDebugHelpers.h"
#include "Physics/ImmediatePhysics/ImmediatePhysicsShared/ImmediatePhysicsCore.h"

// Sets default values
ABallPawn::ABallPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SetRootComponent(StaticMeshComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("ArrowComponent");
	ArrowComponent->SetupAttachment(StaticMeshComponent);
}

// Called when the game starts or when spawned
void ABallPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABallPawn::Tick(float DeltaTime)
{
	if(!IsBallMoving() && bBallMoving)
	{
		ArrowComponent->SetVisibility(true);
	}
	bBallMoving = IsBallMoving();
	
	if(!bBallMoving && !bCameraMoving)
	{
		HitPreview();
	}
	
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABallPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Hit", IE_Pressed, this, &ABallPawn::Hit);
	PlayerInputComponent->BindAction("MoveCamera", IE_Pressed, this, &ABallPawn::StartMovingCamera);
	PlayerInputComponent->BindAction("MoveCamera", IE_Released, this, &ABallPawn::StopMovingCamera);
	PlayerInputComponent->BindAxis("Pitch", this, &ABallPawn::PitchCamera);
	PlayerInputComponent->BindAxis("Yaw", this, &ABallPawn::YawCamera);
	PlayerInputComponent->BindAxis("Zoom", this, &ABallPawn::Zoom);
}

FVector ABallPawn::GetDesiredLocation() const
{
	const auto PlayerController = GetController<APlayerController>();

	if(!PlayerController)
	{
		UE_LOG(BallPawnCategory, Display, TEXT("Pawn is not controlled"));
		return FVector(0, 0, 0);
	}

	float MouseX;
	float MouseY;
	PlayerController->GetMousePosition(MouseX, MouseY);

	FVector WorldPosition;
	FVector WorldDirection;
	if(!UGameplayStatics::DeprojectScreenToWorld(PlayerController, FVector2D(MouseX, MouseY), WorldPosition, WorldDirection))
	{
		UE_LOG(BallPawnCategory, Warning, TEXT("Something went wrong!"));
		return FVector(0, 0, 0);
	}

	UE_LOG(BallPawnCategory, Display, TEXT("World position: ( %f ; %f ; %f )"), WorldPosition.X, WorldPosition.Y, WorldPosition.Z);
	UE_LOG(BallPawnCategory, Display, TEXT("World direction: ( %f ; %f ; %f )"), WorldDirection.X, WorldDirection.Y, WorldDirection.Z);
	
	FVector CameraLocation;
	FRotator CameraViewRotation;
	Controller->GetPlayerViewPoint(CameraLocation, CameraViewRotation);

	UE_LOG(BallPawnCategory, Display, TEXT("Camera location: ( %f ; %f ; %f )"), CameraLocation.X, CameraLocation.Y, CameraLocation.Z);
	UE_LOG(BallPawnCategory, Display, TEXT("Camera view rotation: ( %f ; %f ; %f )"), CameraViewRotation.Vector().X, CameraViewRotation.Vector().Y, CameraViewRotation.Vector().Z);
		
	const auto Plane = UE::Math::TPlane<double>(FVector(0, 0, 1.0f), GetActorLocation().Z);
	
	const FVector Intersection = FMath::RayPlaneIntersection(CameraLocation, WorldDirection, Plane);
	
	UE_LOG(BallPawnCategory, Display, TEXT("Intersection: ( %f ; %f ; %f )"), Intersection.X, Intersection.Y, Intersection.Z);

	return Intersection;
}

bool ABallPawn::IsBallMoving() const
{
	return !GetVelocity().IsNearlyZero();
}

void ABallPawn::Hit()
{
	if(bBallMoving || bCameraMoving) return;

	const FVector DesiredLocation = GetDesiredLocation();
	const FVector ActorLocation = GetActorLocation();

	FVector ImpulseVector = DesiredLocation - ActorLocation;
	ImpulseVector.Normalize();
	ImpulseVector *= FMath::Clamp((DesiredLocation - ActorLocation).Length(), 0, MaxVectorLength);
	ImpulseVector *= HitForce;

	UE_LOG(BallPawnCategory, Display, TEXT("Adding impulse to StaticMeshComponent: ( %f ; %f ; %f )"), ImpulseVector.X, ImpulseVector.Y, ImpulseVector.Z);
	StaticMeshComponent->AddImpulse(ImpulseVector);
	ArrowComponent->SetVisibility(false);
}

void ABallPawn::HitPreview() const
{
	const FVector DesiredLocation = GetDesiredLocation();

	const FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DesiredLocation);
	UE_LOG(BallPawnCategory, Display, TEXT("Look at rotator: ( %f ; %f ; %f )"), LookAtRotator.Yaw, LookAtRotator.Pitch, LookAtRotator.Roll);
	ArrowComponent->SetWorldRotation(LookAtRotator);

	UE_LOG(BallPawnCategory, Display, TEXT("Clamping Arrow with length %f to length %f"), (DesiredLocation - GetActorLocation()).Length(), MaxVectorLength);
	ArrowComponent->ArrowLength = FMath::Clamp((DesiredLocation - GetActorLocation()).Length() / ArrowComponent->ArrowSize, 0, MaxVectorLength);
	ArrowComponent->SetVisibility(false);
	ArrowComponent->SetVisibility(true);
	UE_LOG(BallPawnCategory, Display, TEXT("New arrow Length %f"), ArrowComponent->ArrowLength);
}

void ABallPawn::StartMovingCamera()
{
	bCameraMoving = true;
	
	ArrowComponent->SetVisibility(false);
	
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);

	UE_LOG(BallPawnCategory, Display, TEXT("Start moving camera"));
}
void ABallPawn::StopMovingCamera()
{
	bCameraMoving = false;

	if(!IsBallMoving())
	{
		ArrowComponent->SetVisibility(true);
	}

	
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
	
	UE_LOG(BallPawnCategory, Display, TEXT("Stop moving camera"));
}

void ABallPawn::PitchCamera(const float Amount)
{
	if(bCameraMoving)
	{
		AddControllerPitchInput(Amount);
		UE_LOG(BallPawnCategory, Display, TEXT("Camera pitch moving by: %f"), Amount);
	}

}
void ABallPawn::YawCamera(const float Amount)
{
	if(bCameraMoving)
	{
		AddControllerYawInput(Amount);
		UE_LOG(BallPawnCategory, Display, TEXT("Camera yaw moving by: %f"), Amount);
	}

}

void ABallPawn::Zoom(const float X)
{
	SpringArmComponent->TargetArmLength += X * ZoomSpeed;
	//UE_LOG(BallPawnCategory, Display, TEXT("Camera zoomed by: %f"), X);
}