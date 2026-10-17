// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionUserWidget.generated.h"

class UKeySettingWidget;
/**
 * 
 */
UCLASS()
class DEADZONE_API UOptionUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	TObjectPtr<UKeySettingWidget> GetKeySettingWidget() { return KeySettingWidget; }
	
protected:
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UKeySettingWidget> KeySettingWidget;
};
