// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEADZONE_API UMainMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	//在当前页面被创建出来的时候执行一次
	virtual void NativeOnInitialized() override;
	//只要当前页面加入到视口中，就会被调用
	virtual void NativeConstruct() override;
	
private:
};
