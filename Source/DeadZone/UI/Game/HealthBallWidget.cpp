// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBallWidget.h"

#include "CommonTextBlock.h"
#include "Components/ProgressBar.h"

void UHealthBallWidget::UpdateHealth(int32 Current, int32 Max)
{
	float Percent = static_cast<float>(Current) / static_cast<float>(Max);
	
	ValueSlot->SetText(FText::AsNumber(Current));
	HPProgress->SetPercent(Percent);
}
