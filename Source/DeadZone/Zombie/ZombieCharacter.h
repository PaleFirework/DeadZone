// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeadZone/Core/DzGameDefines.h"
#include "DeadZone/Interface/DeadInterface.h"
#include "GameFramework/Character.h"
#include "ZombieCharacter.generated.h"

class UBehaviorTree;

UCLASS()
class DEADZONE_API AZombieCharacter : public ACharacter, public IDeadInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombieCharacter();
	
	void RotateToTarget(AActor* TargetActor, float DeltaSeconds);
	
	UAnimMontage* PlayTurnMontage(float Angle);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void PlayRandomSound();
	void OnDead();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	TObjectPtr<UBehaviorTree> GetBehaviorTree() const { return BehaviorTree; }
	
	virtual bool IsDead() const override;
	
	ETeamType GetTeamType() const {return TeamType;};
	
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY(EditAnywhere)
	ETeamType TeamType = ETeamType::ETT_Zombie;
	
	int32 CurHP;
	UPROPERTY(EditAnywhere)
	int32 MaxHP = 120;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> HurtEffect;
	UPROPERTY(EditAnywhere)
	TArray<USoundCue*> ZombieAudioCues;
	UPROPERTY()
	TObjectPtr<UAudioComponent> CurrentZombieAudioComponent;
	FTimerHandle SoundTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation|Turn")
	TObjectPtr<UAnimMontage> TurnLeft90Montage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation|Turn")
	TObjectPtr<UAnimMontage> TurnRight90Montage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation|Turn")
	TObjectPtr<UAnimMontage> TurnLeft180Montage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation|Turn")
	TObjectPtr<UAnimMontage> TurnRight180Montage;
	
};
