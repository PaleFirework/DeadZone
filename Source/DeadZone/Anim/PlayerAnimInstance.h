// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class ADzCharacterBase;
/**
 * 
 */
UCLASS()
class DEADZONE_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	virtual void NativeInitializeAnimation() override;
	
protected:
	UFUNCTION()
	void AnimNotify_RelaxedEnd();
	
protected:
	UPROPERTY()
	TObjectPtr<ADzCharacterBase> OwnerCharacter;
	UPROPERTY(BlueprintReadOnly)
	FVector Velocity;
	UPROPERTY(BlueprintReadOnly)
	float Speed;
	UPROPERTY(BlueprintReadOnly)
	bool bShouldMove;
	UPROPERTY(BlueprintReadOnly)
	bool bSprint;
	UPROPERTY(BlueprintReadOnly)
	bool bIsFalling;
	UPROPERTY(BlueprintReadOnly)
	bool bCrouched;
	UPROPERTY(BlueprintReadOnly)
	bool bRelaxed;
	UPROPERTY(EditAnywhere)
	float RelaxedTime = 5.f;
	UPROPERTY(BlueprintReadOnly)
	bool bHoldWeapon;
	UPROPERTY(BlueprintReadOnly)
	float Direction;
	UPROPERTY(BlueprintReadOnly)
	bool bIronsight;
	UPROPERTY(BlueprintReadOnly)
	float AimPitch;
	UPROPERTY(BlueprintReadOnly)
	FVector HoldWeaponPosition;
	UPROPERTY(BlueprintReadOnly)
	bool bFiring;
	
	float RelaxedTick;
};
