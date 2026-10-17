// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ZombieAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DEADZONE_API UZombieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	virtual void NativeInitializeAnimation() override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int RandomBSIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float FwdAxisValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxFwdAxisValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool Turning;
};
