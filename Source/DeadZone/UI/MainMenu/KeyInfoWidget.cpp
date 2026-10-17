// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyInfoWidget.h"

#include "KeySettingWidget.h"
#include "OptionUserWidget.h"
#include "Components/Button.h"
#include "Components/InputKeySelector.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "DeadZone/GamePlay/MainMenu/MainMenuHUD.h"

void UKeyInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	InputKeySelector->OnKeySelected.AddDynamic(this, &ThisClass::OnKeySelected);
}

void UKeyInfoWidget::OnKeySelected(FInputChord SelectedKey)
{
	if (CurrentKey == SelectedKey.Key)
	{
		return;//说明当前选择的和之前的一样，不需要继续执行
	}
	
	//查重，是否有人已经使用新的输入按键
	//不一样，则需要存档
	if (AMainMenuHUD* Hud = Cast<AMainMenuHUD>(GetOwningPlayer()->GetHUD()))
	{
		if (Hud->GetOptionUI()->GetKeySettingWidget()->CheckKeysRepeat(SelectedKey.Key))
		{
			InputKeySelector->SetSelectedKey(CurrentKey);
		} 
		else
		{
			Hud->GetOptionUI()->GetKeySettingWidget()->SaveUserCustomKey(KeyEventName, SelectedKey.Key);
			CurrentKey = SelectedKey.Key;
			// BinButton->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UKeyInfoWidget::SetKeyInfoPanel(FName InKeyEventName, FText KeyDescription, FKey DefaultKey)
{
	KeyDescriptionTextBlock->SetText(KeyDescription);
	InputKeySelector->SetSelectedKey(DefaultKey);
	CurrentKey = DefaultKey;
	KeyEventName = InKeyEventName;
}

void UKeyInfoWidget::ResetDefaultKey(FKey DefaultKey)
{
	if (CurrentKey != DefaultKey)//说明当前的按键和默认的不一致
	{
		CurrentKey = DefaultKey;
		InputKeySelector->SetSelectedKey(DefaultKey);
	}
}
