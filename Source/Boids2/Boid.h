// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid.generated.h"

UCLASS()
class BOIDS2_API ABoid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoid();
	FVector currentVelocity = FVector::ZeroVector;
	class ABoidManager* manager;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	FVector Seek(FVector position);
	FVector Flee(FVector position);

	FVector Wander(float radius, float distance, float jitter);
	FVector wanderDestination = FVector::ZeroVector;

	FVector Pursue(ABoid boid);
	FVector Evade(ABoid boid);
	//FVector Arrive(FVector position);
	
	FVector Seperation(TArray<class ABoid*> neighbours);
	FVector Cohesion(TArray<class ABoid*> neighbours);
	FVector Allignment(TArray<class ABoid*> neighbours);

	UStaticMeshComponent* mesh;
	float speedMult;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void UpdateBoid(float DeltaTime);
};
