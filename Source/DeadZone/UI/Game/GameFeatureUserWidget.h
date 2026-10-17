// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFeatureUserWidget.generated.h"

class UHealthBallWidget;
class UImage;
class UTextBlock;
class ADzPlayerCharacter;
/**
 * 
 */
UCLASS()
class DEADZONE_API UGameFeatureUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	
	void OnEquipWeapon(int32 ID);
	void OnUnEquipWeapon(int32 ID);
	
	void OnWeaponClipChanged(int32 Current, int32 Max);
	
	void OnPlayerHPChanged(int32 Current, int32 Max);
	
public:
	
	void SetGamePlayerPawn(ADzPlayerCharacter* Player);
	
protected:
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> WeaponClipTextBlock;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> WeaponClipIcon;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UHealthBallWidget> HealthBall;public:
	UPROPERTY(Transient, meta = (Transient,BindWidgetAnim))
	UWidgetAnimation* FadeIn;
	UPROPERTY(Transient, meta = (Transient,BindWidgetAnim))
	UWidgetAnimation* FadeOut;
};
