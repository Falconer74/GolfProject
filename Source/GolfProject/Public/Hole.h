// Golf diploma work. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hole.generated.h"

class USphereComponent;

DEFINE_LOG_CATEGORY_STATIC(HoleCategory, All, All);

UCLASS()
class GOLFPROJECT_API AHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHole();

protected:
	UFUNCTION()
	void GoalEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USphereComponent* Collider;
};
