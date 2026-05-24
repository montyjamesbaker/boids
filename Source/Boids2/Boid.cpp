// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"
#include "BoidManager.h"

// Sets default values
ABoid::ABoid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SPHERE"));
	UStaticMesh* sphereMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'")).Object;

	mesh->SetStaticMesh(sphereMesh);
	this->SetRootComponent(mesh);
}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
	speedMult = 5;
}

// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoid::UpdateBoid(float DeltaTime) {
	FVector targetVelocity = FVector::ZeroVector;

	// Find velocity & add steering behaviours
	TArray<ABoid*> closestBoids = manager->GetBoidNeighborhood(this); 
	
	targetVelocity += Seperation(closestBoids) * manager->seperationWeight;
	targetVelocity += Cohesion(closestBoids) * manager->cohesionWeight;
	targetVelocity += Allignment(closestBoids) * manager->allignmentWeight;

	targetVelocity.Normalize();

	if (targetVelocity.Size() < 1.0f) {
		targetVelocity += Wander(100.0f, 1000.0f, 50.0f);
		targetVelocity.Normalize();
	}

	if ((GetActorLocation() - manager->GetActorLocation()).Size() > manager->spawnRadius) {
		targetVelocity += Seek(manager->GetActorLocation());
		targetVelocity.Normalize();
	}


	FVector newForce = targetVelocity - currentVelocity;
	currentVelocity += newForce * DeltaTime;

	FVector location = GetActorLocation();
	location += (currentVelocity * speedMult);

	SetActorLocation(location);
}



FVector ABoid::Seek(FVector position) {
	FVector newVelocity = position - GetActorLocation();
	newVelocity.Normalize();
	return newVelocity;
}
FVector ABoid::Flee(FVector position) {
	FVector newVelocity = GetActorLocation() - position;
	newVelocity.Normalize();
	return newVelocity;
}

FVector ABoid::Wander(float radius, float distance, float jitter)
{
	FVector myLocation = GetActorLocation();
	if (FVector::Dist(myLocation, wanderDestination) < 100.0f) {
		FVector projectedPos = myLocation + (GetActorForwardVector() * distance);
		wanderDestination = projectedPos + (FMath::VRand() * FMath::RandRange(0.0f, jitter));
	}
	FVector jitterDestination = Seek(wanderDestination) + (FMath::VRand() * FMath::RandRange(0.0f, jitter));
	return jitterDestination;
}

FVector ABoid::Pursue(ABoid boid) {
	FVector newVelocity = boid.GetActorLocation() + boid.currentVelocity - GetActorLocation();
	newVelocity.Normalize();
	return newVelocity;
}
FVector ABoid::Evade(ABoid boid) {
	FVector newVelocity = GetActorLocation() - boid.GetActorLocation() - boid.currentVelocity;
	newVelocity.Normalize();

	return newVelocity;
}

FVector ABoid::Seperation(TArray<class ABoid*> neighbours) {
	FVector newVelocity;
	FVector averagePosition;
	if (neighbours.Num() == 0) {
		return FVector::ZeroVector;
	}
	for (ABoid* boid : neighbours) {
		averagePosition += boid->GetActorLocation();
	}
	averagePosition /= neighbours.Num();
	newVelocity = Flee(averagePosition);
	return newVelocity;
}

FVector ABoid::Cohesion(TArray<class ABoid*> neighbours) {
	FVector newVelocity;
	FVector averagePosition;
	if (neighbours.Num() == 0) {
		return FVector::ZeroVector;
	}
	for (ABoid* boid : neighbours) {
		averagePosition += boid->GetActorLocation();
	}
	averagePosition /= neighbours.Num();
	newVelocity = Seek(averagePosition);
	return newVelocity;
}

FVector ABoid::Allignment(TArray<class ABoid*> neighbours) {
	FVector newVelocity;
	if (neighbours.Num() == 0) {
		return FVector::ZeroVector;
	}
	for (ABoid* boid : neighbours) {
		newVelocity += boid->currentVelocity;
	}
	newVelocity /= neighbours.Num(); // test with and without later
	newVelocity.Normalize();
	return newVelocity;
}

