// Golf diploma work. All rights reserved


#include "BallPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABallPawn::ABallPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(GetRootComponent());

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
	float mouseX;
	float mouseY;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(mouseX, mouseY);
	
	// UE_LOG(BallPawnCategory, Display, TEXT("Mouse position: ( %f ; %f" ), mouseX, mouseY);
	
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABallPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Hit", IE_Pressed, this, &ABallPawn::Hit);
	PlayerInputComponent->BindAction("MoveCamera", IE_Pressed, this, &ABallPawn::StartMovingCamera);
	PlayerInputComponent->BindAction("MoveCamera", IE_Pressed, this, &ABallPawn::StopMovingCamera);
	PlayerInputComponent->BindAxis("Pitch", this, &ABallPawn::PitchCamera);
	PlayerInputComponent->BindAxis("Yaw", this, &ABallPawn::YawCamera);
	PlayerInputComponent->BindAxis("Zoom", this, &ABallPawn::Zoom);
}

void ABallPawn::Hit()
{
	
}

void ABallPawn::StartMovingCamera()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);

	UE_LOG(BallPawnCategory, Warning, TEXT("Start moving camera"));
}
void ABallPawn::StopMovingCamera()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
	
	UE_LOG(BallPawnCategory, Warning, TEXT("Stop moving camera"));
}

void ABallPawn::PitchCamera(const float Amount)
{
	AddControllerPitchInput(Amount);

	// UE_LOG(BallPawnCategory, Warning, TEXT("AddPitchInput"));
}
void ABallPawn::YawCamera(const float Amount)
{
	AddControllerYawInput(Amount);

	// UE_LOG(BallPawnCategory, Display, TEXT("AddYawInput"));
}

void ABallPawn::Zoom(float X)
{
	SpringArmComponent->TargetArmLength += X * ZoomSpeed;
}

