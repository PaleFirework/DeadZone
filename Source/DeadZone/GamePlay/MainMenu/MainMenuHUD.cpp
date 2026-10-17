// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuHUD.h"

#include "DeadZone/UI/MainMenu/MainMenuUserWidget.h"
#include "DeadZone/UI/MainMenu/OptionUserWidget.h"

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	TSubclassOf<UMainMenuUserWidget> WidgetClass = LoadClass<UMainMenuUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/DeadZone/UI/MainMenu/UMG_MainMenu.UMG_MainMenu_C'"));
	if (WidgetClass)
	{
		//创建UserWidget实例
		UMainMenuUserWidget* WidgetObject = CreateWidget<UMainMenuUserWidget>(GetOwningPlayerController(), WidgetClass);
		if (WidgetObject)
		{
			WidgetObject->AddToViewport();
		}
	}
	GetOwningPlayerController()->bShowMouseCursor = true;
	
}

void AMainMenuHUD::ShowOptionUI()
{
	if (!OptionUserWidget)
	{
		
		TSubclassOf<UOptionUserWidget> WidgetClass = LoadClass<UOptionUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/DeadZone/UI/MainMenu/UMG_Option.UMG_Option_C'"));
		OptionUserWidget = CreateWidget<UOptionUserWidget>(GetOwningPlayerController(), WidgetClass);
	}
	if (OptionUserWidget)
	{
		OptionUserWidget->AddToViewport();
	}
}
