// Golf diploma work. All rights reserved


#include "GolfProjectPlayerController.h"
#include "BallPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GolfPlayerState.h"
#include "GolfProjectHUDBase.h"
#include "Kismet/GameplayStatics.h"

FVector AGolfProjectPlayerControllerBase::GetDesiredLocation(const float MouseX, const float MouseY) const
{
	FVector WorldPosition;
	FVector WorldDirection;
	checkf(UGameplayStatics::DeprojectScreenToWorld(this, FVector2D(MouseX, MouseY), WorldPosition, WorldDirection),
		TEXT("GolfProjectPlayerController::GetDesiredLocation(), something went wrong"));

	WorldDirection.Z = FMath::Clamp(WorldDirection.Z, -1, -0.00001);

	FVector  CameraLocation;
	FRotator CameraViewRotation;
	GetPlayerViewPoint(CameraLocation, CameraViewRotation);

	const auto Plane = UE::Math::TPlane<double>(FVector(0, 0, 1.0f), BallPawn->GetActorLocation().Z);

	const FVector Intersection = FMath::RayPlaneIntersection(CameraLocation, WorldDirection, Plane);

	return Intersection;
}

void AGolfProjectPlayerControllerBase::LookAround(const FInputActionValue& InputActionValue)
{
	const FVector2d LookAxis = InputActionValue.Get<FVector2d>() * CameraSensitivity;

	UE_LOG(GolfPlayerControllerCategory, Display, TEXT("Looking around x:%f   y:%f"), LookAxis.X, LookAxis.Y);

	AddYawInput(LookAxis.X);
	AddPitchInput(LookAxis.Y);
}

void AGolfProjectPlayerControllerBase::ChooseHitDirection()
{
	float MouseX;
	float MouseY;

	if (!GetMousePosition(MouseX, MouseY))
	{
		return;
	}

	BallPawn->HitPreview(GetDesiredLocation(MouseX, MouseY));
}

void AGolfProjectPlayerControllerBase::HitBall()
{
	float MouseX;
	float MouseY;
	checkf(GetMousePosition(MouseX, MouseY), TEXT("GolfProjectPlayerController:GetMousePosition returned false, no associated mouse device"));

	BallPawn->Hit(GetDesiredLocation(MouseX, MouseY));

	// Hit counter prediction
	GolfPlayerState->AddHit();
}

void AGolfProjectPlayerControllerBase::Zoom(const FInputActionValue& InputActionValue)
{
	const float ZoomAmount = InputActionValue.Get<float>() * ZoomSpeed;

	BallPawn->Zoom(ZoomAmount);
}

void AGolfProjectPlayerControllerBase::BallMovingContext(ABallPawn* Ball)
{
	bLookingAround = true;
	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(MovingBallInputContext, 0);

	SetShowMouseCursor(!bLookingAround);
}

void AGolfProjectPlayerControllerBase::BallStopMovingContext(ABallPawn* Ball)
{
	bLookingAround = false;
	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(StationaryBallBasicInputContext, 0);
	InputSubsystem->AddMappingContext(ChoosingDirectionInputContext, 0);

	SetShowMouseCursor(!bLookingAround);
	ChooseHitDirection();
}

void AGolfProjectPlayerControllerBase::ToggleOnLookAround(const FInputActionInstance& InputActionInstance)
{
	bLookingAround = true;

	FModifyContextOptions Options;
	Options.bForceImmediately = true;
	Options.bIgnoreAllPressedKeysUntilRelease = false;

	InputSubsystem->RemoveMappingContext(ChoosingDirectionInputContext, Options);
	InputSubsystem->AddMappingContext(LookingAroundInputContext, 0, Options);

	BallPawn->SetArrowVisibility(!bLookingAround);
	SetShowMouseCursor(!bLookingAround);

	UE_LOG(GolfPlayerControllerCategory, Display, TEXT("Toggle look around: Started"));
}

void AGolfProjectPlayerControllerBase::ToggleOffLookAround(const FInputActionInstance& InputActionInstance)
{
	bLookingAround = false;

	FModifyContextOptions Options;
	Options.bForceImmediately = true;
	Options.bIgnoreAllPressedKeysUntilRelease = false;

	InputSubsystem->RemoveMappingContext(LookingAroundInputContext, Options);
	InputSubsystem->AddMappingContext(ChoosingDirectionInputContext, 0, Options);

	BallPawn->SetArrowVisibility(!bLookingAround);
	SetShowMouseCursor(!bLookingAround);
	ChooseHitDirection();

	UE_LOG(GolfPlayerControllerCategory, Display, TEXT("Toggle look around: Completed"));
}

void AGolfProjectPlayerControllerBase::BallHit(ABallPawn* Ball)
{
	GolfPlayerState->AddHit();
}

void AGolfProjectPlayerControllerBase::PowerChanged(ABallPawn* Ball, float Power, float MaxPower)
{
	GolfHUD->SetPowerBarValue(Power, MaxPower);
}

void AGolfProjectPlayerControllerBase::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	BallPawn = Cast<ABallPawn>(aPawn);
	BallPawn->OnBallHit.AddDynamic(this, &AGolfProjectPlayerControllerBase::BallHit);

	GolfPlayerState = Cast<AGolfPlayerState>(PlayerState);
	checkf(GolfPlayerState, TEXT("GolfPlayerState in AGolfProjectPlayerControllerBase::OnPossess(APawn* aPawn) is nullptr"));

	BallPawn = Cast<ABallPawn>(aPawn);
	checkf(BallPawn, TEXT("BallPawn is nullptr"));
}

void AGolfProjectPlayerControllerBase::OnUnPossess()
{
	Super::OnUnPossess();

	PlayerControllerClientClean();
}

void AGolfProjectPlayerControllerBase::PlayerControllerClientClean_Implementation()
{
	EnhancedInputComponent->ClearActionBindings();
	
	GolfHUD->HideGameplayWidget();
}

UNetDriver* AGolfProjectPlayerControllerBase::GetNetDriverHelper() const
{
	return GetNetDriver();
}

void AGolfProjectPlayerControllerBase::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);
	
	BallPawn = Cast<ABallPawn>(P);
	
	GolfHUD = Cast<AGolfProjectHUDBase>(GetHUD());
	checkf(GolfHUD, TEXT("GolfHUD in AGolfProjectPlayerControllerBase::PlayerControllerClientSetup_Implementation() is nullptr"));
	
	GolfPlayerState = Cast<AGolfPlayerState>(PlayerState);
	checkf(GolfPlayerState, TEXT("GolfPlayerState in AGolfProjectPlayerControllerBase::OnPossess(APawn* aPawn) is nullptr"));

	SetShowMouseCursor(!bLookingAround);

	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	checkf(EnhancedInputComponent, TEXT("EnhancedInputComponent is nullptr"));

	InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	checkf(InputSubsystem, TEXT("InputSubsystem is nullptr"));

	BallPawn->OnBallStartMoving.AddDynamic(this, &AGolfProjectPlayerControllerBase::BallMovingContext);
	BallPawn->OnBallStopMoving.AddDynamic(this, &AGolfProjectPlayerControllerBase::BallStopMovingContext);

	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(StationaryBallBasicInputContext, 0);
	InputSubsystem->AddMappingContext(ChoosingDirectionInputContext, 0);

	if (LookAroundAction)
	{
		EnhancedInputComponent->BindAction(LookAroundAction, ETriggerEvent::Triggered,
			this, &AGolfProjectPlayerControllerBase::LookAround);
	}

	if (ChooseDirectionAction)
	{
		EnhancedInputComponent->BindAction(ChooseDirectionAction, ETriggerEvent::Triggered,
			this, &AGolfProjectPlayerControllerBase::ChooseHitDirection);
	}

	if (HitAction)
	{
		EnhancedInputComponent->BindAction(HitAction, ETriggerEvent::Triggered,
			this, &AGolfProjectPlayerControllerBase::HitBall);
	}

	if (ToggleLookAroundAction)
	{
		EnhancedInputComponent->BindAction(ToggleLookAroundAction, ETriggerEvent::Completed,
			this, &AGolfProjectPlayerControllerBase::ToggleOffLookAround);
	}

	if (ToggleLookAroundAction)
	{
		EnhancedInputComponent->BindAction(ToggleLookAroundAction, ETriggerEvent::Started,
			this, &AGolfProjectPlayerControllerBase::ToggleOnLookAround);
	}

	if (ZoomAction)
	{
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered,
			this, &AGolfProjectPlayerControllerBase::Zoom);
	}
	
	BallPawn->OnPowerChanged.AddDynamic(this, &AGolfProjectPlayerControllerBase::PowerChanged);
	GolfPlayerState->OnHitCounterChanged.AddDynamic(GolfHUD, &AGolfProjectHUDBase::SetHitCounterValue);

	GolfHUD->ShowGameplayWidget();
}
