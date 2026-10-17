// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PromptWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEADZONE_API UPromptWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetPromptText(const FText& NewText);
};
