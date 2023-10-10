// Golf diploma work. All rights reserved


#include "BallPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "GolfProjectPlayerController.h"
#include "GolfGameplayWidget.h"
#include "Blueprint/UserWidget.h"
#include "DrawDebugHelpers.h"
#include "GolfPlayerState.h"

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

	OnBallStartMoving.AddDynamic(this, &ABallPawn::ChangeArrowVisibility);
	OnBallStopMoving.AddDynamic(this, &ABallPawn::ChangeArrowVisibility);

	if(IsLocallyControlled() && GameplayWidgetClass)
	{
		AGolfProjectPlayerControllerBase* GolfController = GetController<AGolfProjectPlayerControllerBase>();
		check(GolfController)
		GameplayWidget = CreateWidget<UGolfGameplayWidget>(GolfController, GameplayWidgetClass);
		check(GameplayWidget);
		GameplayWidget->AddToPlayerScreen();
	}
}

void ABallPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(GameplayWidget)
	{
		GameplayWidget->RemoveFromParent();
		GameplayWidget = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

void ABallPawn::ChangeArrowVisibility(ABallPawn* Ball)
{
	SetArrowVisibility(!bBallMoving);
}

void ABallPawn::SetArrowVisibility(bool bVisible)
{
	ArrowComponent->SetVisibility(bVisible);
}

// Called every frame
void ABallPawn::Tick(float DeltaTime)
{
	if(!bBallMoving && IsBallMoving())
	{
		bBallMoving = true;

		OnBallHit.Broadcast(this);
		OnBallStartMoving.Broadcast(this);
	}
	else if(bBallMoving && !IsBallMoving())
	{
		bBallMoving = false;
		
		OnBallStopMoving.Broadcast(this);
	}
	
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABallPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool ABallPawn::IsBallMoving() const
{
	return !(GetVelocity().IsNearlyZero());
}

void ABallPawn::Hit(const FVector& DesiredLocation)
{
	if(bBallMoving) return;

	const FVector ActorLocation = GetActorLocation();

	FVector ImpulseVector = DesiredLocation - ActorLocation;
	ImpulseVector.Normalize();
	ImpulseVector *= FMath::Clamp((DesiredLocation - ActorLocation).Length(), 0, MaxVectorLength);
	ImpulseVector *= HitForce;

	UE_LOG(BallPawnCategory, Display, TEXT("Adding impulse to StaticMeshComponent: ( %f ; %f ; %f )"), ImpulseVector.X, ImpulseVector.Y, ImpulseVector.Z);
	StaticMeshComponent->AddImpulse(ImpulseVector, NAME_None, true);

	OnBallHit.Broadcast(this);

	// TODO Rewrite with delegates
	AGolfPlayerState* GolfPlayerState = Cast<AGolfPlayerState>(Controller->PlayerState);
	GolfPlayerState->AddHit();
	GameplayWidget->SetHitCount(GolfPlayerState->GetHitsCount());
}

void ABallPawn::HitPreview(const FVector& DesiredLocation) const
{
	const FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DesiredLocation);
	UE_LOG(BallPawnCategory, Display, TEXT("Look at rotator: ( %f ; %f ; %f )"), LookAtRotator.Yaw, LookAtRotator.Pitch, LookAtRotator.Roll);
	ArrowComponent->SetWorldRotation(LookAtRotator);

	UE_LOG(BallPawnCategory, Display, TEXT("Clamping Arrow with length %f to length %f"), (DesiredLocation - GetActorLocation()).Length(), MaxVectorLength);

	const float Power = FMath::Clamp((DesiredLocation - GetActorLocation()).Length() / ArrowComponent->ArrowSize, 0, MaxVectorLength);
	GameplayWidget->SetPower(Power, MaxVectorLength);
	
	ArrowComponent->ArrowLength = Power;
	ArrowComponent->SetVisibility(false);
	ArrowComponent->SetVisibility(true);
	UE_LOG(BallPawnCategory, Display, TEXT("New arrow Length %f"), ArrowComponent->ArrowLength);
}

void ABallPawn::Zoom(const float X)
{
	SpringArmComponent->TargetArmLength += X;
}