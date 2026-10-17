// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

class UOptionUserWidget;
/**
 * 
 */
UCLASS()
class DEADZONE_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
public:
	
	UFUNCTION(BlueprintCallable)
	void ShowOptionUI();
	
	TObjectPtr<UOptionUserWidget> GetOptionUI() { return OptionUserWidget; }
protected:
	
	UPROPERTY()
	TObjectPtr<UOptionUserWidget> OptionUserWidget;
};
