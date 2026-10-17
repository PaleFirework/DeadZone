// Fill out your copyright notice in the Description page of Project Settings.


#include "DzPlayerController.h"

#include "DeadZone/Camera/DzPlayerCameraManager.h"

ADzPlayerController::ADzPlayerController()
{
	PlayerCameraManagerClass = ADzPlayerCameraManager::StaticClass();
}

void ADzPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}
