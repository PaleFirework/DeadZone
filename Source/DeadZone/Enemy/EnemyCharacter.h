// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeadZone/Character/DzCharacterBase.h"
#include "EnemyCharacter.generated.h"

class UBehaviorTree;

UCLASS()
class DEADZONE_API AEnemyCharacter : public ADzCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnDead() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	TObjectPtr<UBehaviorTree> GetBehaviorTree() const { return BehaviorTree; }
	
protected:
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> BehaviorTree;
};
