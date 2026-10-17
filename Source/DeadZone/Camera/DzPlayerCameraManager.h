// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "DzPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class DEADZONE_API ADzPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
protected:
	
	virtual void UpdateCamera(float DeltaTime) override;
	
	bool bLastCrouch;//记录上一帧的蹲伏状态
	
	float TargetSpringOffsetZ;
};
