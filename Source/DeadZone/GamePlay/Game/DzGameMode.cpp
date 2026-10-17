// Fill out your copyright notice in the Description page of Project Settings.


#include "DzGameMode.h"

#include "DzHUD.h"
#include "DeadZone/Player/DzPlayerCharacter.h"
#include "DeadZone/Player/DzPlayerController.h"

ADzGameMode::ADzGameMode()
{
	ConstructorHelpers::FClassFinder<ADzPlayerCharacter> PlayerClass(TEXT("/Game/DeadZone/Blueprints/Player/BP_Player"));
	DefaultPawnClass = PlayerClass.Class;
	
	PlayerControllerClass = ADzPlayerController::StaticClass();
	HUDClass = ADzHUD::StaticClass();
}

int32 ADzGameMode::GetDamageValueWithBoneName(FName BoneName)
{
	if (BoneName == TEXT("head"))
	{
		return FMath::RandRange(80, 90);
	}
	else if (BoneName == TEXT("spine_01") || BoneName == TEXT("spine_02") || BoneName == TEXT("spine_03"))
	{
		return FMath::RandRange(20, 30);
	}
	return FMath::RandRange(5, 10);
}
