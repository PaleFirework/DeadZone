// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAnimInstance.h"

void UZombieAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UZombieAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	MaxFwdAxisValue = FMath::FRandRange(2.5f, 3.0f);
	RandomBSIndex = FMath::RandRange(0, 2);
}
