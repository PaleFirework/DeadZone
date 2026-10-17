// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "DzGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class DEADZONE_API UDzGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	float GetMouseSpeed() const;
	
protected:
	
	UPROPERTY(BlueprintReadWrite, Config)
	float MouseSensitivity = 0.5f;
};
