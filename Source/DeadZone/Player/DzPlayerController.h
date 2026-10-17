// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DzPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DEADZONE_API ADzPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	ADzPlayerController();
	
protected:
	
	virtual void SetupInputComponent() override;
};
