// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "KeyMappingSaveGame.generated.h"

/**
 * 用来存储用户的自定义按键
 */
UCLASS()
class DEADZONE_API UKeyMappingSaveGame : public USaveGame
{
	GENERATED_BODY()
	
	
public:
	
	UPROPERTY()
	TMap<FName, FKey> CustomKeyMap;
	
};
