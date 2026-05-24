// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidManager.generated.h"

UCLASS()
class BOIDS2_API ABoidManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidManager();
	
	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category="Settings")
	int spawnCount = 30;
	
	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "Settings")
	int spawnRadius = 30;

	UFUNCTION(BlueprintCallable)
	void setSpawnRadius(int r) { spawnRadius = r; }

	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float neighbourRadius = 1000;

	UFUNCTION(BlueprintCallable)
	void setNeighbourRadius(float r) { neighbourRadius = r; }

	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "Boid Weights")
	float seperationWeight = 1;

	UFUNCTION(BlueprintCallable)
	void setSeperationWeight(float r) { seperationWeight = r; }

	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "Boid Weights")
	float cohesionWeight = 1;

	UFUNCTION(BlueprintCallable)
	void setCohesionWeight(float r) { cohesionWeight = r; }

	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere, Category = "Boid Weights")
	float allignmentWeight = 1;

	UFUNCTION(BlueprintCallable)
	void setAllignmentWeight(float r) { allignmentWeight = r; }

	USceneComponent* transform;

	TArray<class ABoid*> myBoids;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<class ABoid*> GetBoidNeighborhood(ABoid* boid);
};
