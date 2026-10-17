// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBallWidget.generated.h"

class UCommonTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class DEADZONE_API UHealthBallWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateHealth(int32 Current, int32 Max);
	
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> HPProgress;	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> ValueSlot;
	
};
