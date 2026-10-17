// Fill out your copyright notice in the Description page of Project Settings.


#include "TooltipWidget.h"

#include "CommonTextBlock.h"
#include "PromptWidget.h"
#include "DeadZone/Core/DzGameDefines.h"
#include "DeadZone/Subsystem/PropsSubsystem.h"

class UPromptWidget;

void UTooltipWidget::UpdateInfo(int32 ID, bool IsNearOrStorage)
{
	if (ID < CONSUMABLE_INDEX)
	{
		LeftPrompt->SetVisibility(ESlateVisibility::Visible);
	} else
	{
		LeftPrompt->SetVisibility(ESlateVisibility::Hidden);
	}
	
	
	if (IsNearOrStorage)
	{
		RightPrompt->SetPromptText(FText::FromString(TEXT("拾 取")));
	} else
	{
		RightPrompt->SetPromptText(FText::FromString(TEXT("丢 弃")));
	}
	
	const FPropsBase* PropsBase = GetWorld()->GetGameInstance()->GetSubsystem<UPropsSubsystem>()->GetProps(ID);
	if (PropsBase)
	{
		ItemName->SetText(PropsBase->Name);
		
		FText InfoFormat = FText::FromString(TEXT("[ 物 品 信 息 ]\n\n这是{0}"));
		ItemInfo->SetText(FText::Format(InfoFormat, PropsBase->Name));
		
		if (PropsBase->bCanAddToPackage)
		{
			RightPrompt->SetVisibility(ESlateVisibility::Visible);
		} else
		{
			RightPrompt->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UTooltipWidget::UpdateInfo(int32 ID)
{
	LeftPrompt->SetVisibility(ESlateVisibility::Hidden);
	RightPrompt->SetVisibility(ESlateVisibility::Visible);
	
	RightPrompt->SetPromptText(FText::FromString(TEXT("卸 下")));
	
	const FPropsBase* PropsBase = GetWorld()->GetGameInstance()->GetSubsystem<UPropsSubsystem>()->GetProps(ID);
	if (PropsBase)
	{
		ItemName->SetText(PropsBase->Name);
		
		FText InfoFormat = FText::FromString(TEXT("[ 物 品 信 息 ]\n\n这是{0}"));
		ItemInfo->SetText(FText::Format(InfoFormat, PropsBase->Name));
	}
}
