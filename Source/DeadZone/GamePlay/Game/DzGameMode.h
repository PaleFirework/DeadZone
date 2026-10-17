// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DzGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DEADZONE_API ADzGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	
	ADzGameMode();
	
	int32 GetDamageValueWithBoneName(FName BoneName);
	
	
};
