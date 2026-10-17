// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

struct FAIStimulus;

UCLASS()
class DEADZONE_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnPossess(APawn* InPawn) override;
	UFUNCTION()
	void OnPerceptionTargetUpdate(AActor* Actor, FAIStimulus Stimulus);
	
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	void HandleOnAttacked(AController* AttackerController);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void NotifyDead();
};
