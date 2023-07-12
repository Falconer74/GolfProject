// Golf diploma work. All rights reserved


#include "Hole.h"

#include "GolfProject/GolfProjectGameModeBase.h"

// Sets default values
AHole::AHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collider = CreateDefaultSubobject<USphereComponent>("Collider");
	SetRootComponent(Collider);
}

void AHole::Goal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(HoleCategory, Display, TEXT("GOAL!"));

	ABallPawn* BallPawn = Cast<ABallPawn>(OtherActor);
	AGolfProjectPlayerController* Controller = Cast<AGolfProjectPlayerController>(BallPawn->Controller);

	Cast<AGolfProjectGameModeBase>(GetWorld()->GetAuthGameMode())->Goal(BallPawn, Controller);
	// OnGoal.Broadcast(BallPawn, Controller);
}

// Called when the game starts or when spawned
void AHole::BeginPlay()
{
	Super::BeginPlay();

	Collider->OnComponentBeginOverlap.AddDynamic(this, &AHole::Goal);
}
