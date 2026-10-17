// Fill out your copyright notice in the Description page of Project Settings.


#include "KeySettingWidget.h"

#include "KeyInfoWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "DeadZone/Core/DzGameDefines.h"
#include "DeadZone/SaveGame/KeyMappingSaveGame.h"
#include "Kismet/GameplayStatics.h"


void UKeySettingWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	KeyMappingDT = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/DeadZone/Data/KeyMapping/DT_KeyMapping.DT_KeyMapping'"));
	if (KeyMappingDT)
	{
		TSubclassOf<UKeyInfoWidget> WidgetClass = LoadClass<UKeyInfoWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/DeadZone/UI/MainMenu/WBP_KeyInfo.WBP_KeyInfo_C'"));
		TMap<FName, uint8*> RowMap = KeyMappingDT->GetRowMap();
		for (auto KV : RowMap)
		{
			FKeyInfoHeader* KeyInfoHeader = reinterpret_cast<FKeyInfoHeader*>(KV.Value);

			if (KeyInfoHeader->KeyDescription.IsEmpty())
			{
				continue;
			}
			
			UKeyInfoWidget* KeyInfoWidget = CreateWidget<UKeyInfoWidget>(GetOwningPlayer(), WidgetClass);
			//检查当前的按键事件是否存在存档，如果存在存档，则返回存档按键使用
			FKey Key = GetCustomKey(KV.Key);
			//检查按键是否存在
			if (!Key.IsValid())
			{
				Key = KeyInfoHeader->DefaultKey;
			}
			KeyInfoWidget->SetKeyInfoPanel(KV.Key, KeyInfoHeader->KeyDescription, Key);
			
			//添加到容器中
			if (KeysLeft->GetChildrenCount() == KeysRight->GetChildrenCount())
			{
				KeysLeft->AddChild(KeyInfoWidget);
			} else {
				KeysRight->AddChild(KeyInfoWidget);
			}
			
		}
	}
}

FKey UKeySettingWidget::GetCustomKey(FName KeyEventName)
{
	FKey Key;//默认这个Key对象就是无效对象
	if (!KeyMappingSaveGame)
	{
		ResetButton->SetVisibility(ESlateVisibility::Hidden);
		//检查是否有存档过
		if (UGameplayStatics::DoesSaveGameExist(CUSTOM_KEY_SAVE_NAME, 0))
		{
			KeyMappingSaveGame = Cast<UKeyMappingSaveGame>(UGameplayStatics::LoadGameFromSlot(CUSTOM_KEY_SAVE_NAME, 0));
		}
	}
	if (KeyMappingSaveGame)
	{
		ResetButton->SetVisibility(ESlateVisibility::Visible);
		if (KeyMappingSaveGame->CustomKeyMap.Contains(KeyEventName))
		{
			Key = KeyMappingSaveGame->CustomKeyMap[KeyEventName];
		}
	}
	return Key;
}

void UKeySettingWidget::ResetAllKeys()
{
	//重置所有按键
	if (KeyMappingDT)
	{
		for (int32 i=0; i<KeysLeft->GetChildrenCount(); ++i)
		{
			if (UKeyInfoWidget* KeyInfoWidget = Cast<UKeyInfoWidget>(KeysLeft->GetChildAt(i)))
			{
				if (KeyMappingDT->GetRowMap().Contains(KeyInfoWidget->GetKeyEventName()))
				{
					//转换数据
					FKeyInfoHeader* KeyInfoHeader = reinterpret_cast<FKeyInfoHeader*>(KeyMappingDT->GetRowMap()[KeyInfoWidget->GetKeyEventName()]);
					//重置按键
					KeyInfoWidget->ResetDefaultKey(KeyInfoHeader->DefaultKey);
				}
			}
		}
		for (int32 i=0; i<KeysRight->GetChildrenCount(); ++i)
		{
			if (UKeyInfoWidget* KeyInfoWidget = Cast<UKeyInfoWidget>(KeysRight->GetChildAt(i)))
			{
				if (KeyMappingDT->GetRowMap().Contains(KeyInfoWidget->GetKeyEventName()))
				{
					//转换数据
					FKeyInfoHeader* KeyInfoHeader = reinterpret_cast<FKeyInfoHeader*>(KeyMappingDT->GetRowMap()[KeyInfoWidget->GetKeyEventName()]);
					//重置按键
					KeyInfoWidget->ResetDefaultKey(KeyInfoHeader->DefaultKey);
				}
			}
		}
	}
	//删除存档
	if (KeyMappingSaveGame)
	{
		UGameplayStatics::DeleteGameInSlot(CUSTOM_KEY_SAVE_NAME, 0);
		//释放U类指针
		KeyMappingSaveGame = nullptr;
	}
}


bool UKeySettingWidget::CheckKeysRepeat(const FKey& SelectedKey) const
{
	for (int i = 0; i < KeysLeft->GetChildrenCount(); ++i)
	{
		if (UKeyInfoWidget* KeyInfoWidget = Cast<UKeyInfoWidget>(KeysLeft->GetChildAt(i)))
		{
			if (SelectedKey == KeyInfoWidget->GetKeyEventKey())
			{
				return true;
			}
		}
	}
	for (int i = 0; i < KeysRight->GetChildrenCount(); ++i)
	{
		if (UKeyInfoWidget* KeyInfoWidget = Cast<UKeyInfoWidget>(KeysRight->GetChildAt(i)))
		{
			if (SelectedKey == KeyInfoWidget->GetKeyEventKey())
			{
				return true;
			}
		}
	}
	return false;
}

void UKeySettingWidget::SaveUserCustomKey(FName KeyEventName, FKey Key)
{
	if (!KeyMappingSaveGame)
	{
		//创建一个存档对象
		KeyMappingSaveGame = Cast<UKeyMappingSaveGame>(UGameplayStatics::CreateSaveGameObject(UKeyMappingSaveGame::StaticClass()));
	}
	//记录数据 
	if (KeyMappingSaveGame->CustomKeyMap.Contains(KeyEventName))
	{
		if (KeyMappingSaveGame->CustomKeyMap[KeyEventName] == Key)
		{
			return;
		}
		KeyMappingSaveGame->CustomKeyMap[KeyEventName] = Key;
	}
	else
	{
		KeyMappingSaveGame->CustomKeyMap.Add(KeyEventName, Key);
	}
	//存档
	UGameplayStatics::SaveGameToSlot(KeyMappingSaveGame, CUSTOM_KEY_SAVE_NAME, 0);
	ResetButton->SetVisibility(ESlateVisibility::Visible);
}
