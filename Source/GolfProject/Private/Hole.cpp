// Golf diploma work. All rights reserved


#include "Hole.h"

#include "Components/SphereComponent.h"

// Sets default values
AHole::AHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Collider = CreateDefaultSubobject<USphereComponent>("Collider");
	Collider->SetupAttachment(Mesh);
}

void AHole::GoalEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(HoleCategory, Display, TEXT("GOAL!"));
}

// Called when the game starts or when spawned
void AHole::BeginPlay()
{
	Super::BeginPlay();
	
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AHole::GoalEvent);
}

