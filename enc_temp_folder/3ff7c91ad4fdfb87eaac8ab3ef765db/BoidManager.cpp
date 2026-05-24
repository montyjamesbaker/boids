// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidManager.h"
#include "Boid.h"

// Sets default values
ABoidManager::ABoidManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	transform = CreateDefaultSubobject<USceneComponent>("Root Scene Component");
	this->SetRootComponent(transform);
}

// Called when the game starts or when spawned
void ABoidManager::BeginPlay()
{
	Super::BeginPlay();
	
	for (int i = 0; i < spawnCount; i++) {
		FVector spawnLocation = (FMath::VRand() * FMath::RandRange(0, spawnRadius)) + GetActorLocation();
		FRotator spawnRotation;

		ABoid* newBoid = GetWorld()->SpawnActor<ABoid>(spawnLocation, spawnRotation);
		myBoids.Add(newBoid);

		newBoid->manager = this;
	}
}

// Called every frame
void ABoidManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (ABoid* boid : myBoids) {
		boid->UpdateBoid(DeltaTime);
	}
}

TArray<class ABoid*> ABoidManager::GetBoidNeighborhood(ABoid* thisBoid)
{
	TArray<class ABoid*> returnBoids;
	for (ABoid* boid : myBoids) {
		if (boid == thisBoid || !boid) {
			continue;
		}
		float aDistance = (boid->GetActorLocation() - thisBoid->GetActorLocation()).Size();
		if (aDistance < neighbourRadius) {
			returnBoids.Add(boid);
		}
	}
	return returnBoids;
}	