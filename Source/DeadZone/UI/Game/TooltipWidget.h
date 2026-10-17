// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TooltipWidget.generated.h"

class UCommonTextBlock;
class UPromptWidget;
/**
 * 
 */
UCLASS()
class DEADZONE_API UTooltipWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> ItemName;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> ItemInfo;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UPromptWidget> LeftPrompt;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UPromptWidget> RightPrompt;
	
public:
	void UpdateInfo(int32 ID, bool IsNearOrStorage);
	void UpdateInfo(int32 ID);
	
};
