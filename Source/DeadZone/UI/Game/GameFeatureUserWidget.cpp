// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatureUserWidget.h"

#include "HealthBallWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DeadZone/Components/PackageComponent.h"
#include "DeadZone/Player/DzPlayerCharacter.h"
#include "DeadZone/Weapon/WeaponActor.h"

void UGameFeatureUserWidget::OnEquipWeapon(int32 ID)
{
	WeaponClipTextBlock->SetVisibility(ESlateVisibility::Visible);
	WeaponClipIcon->SetVisibility(ESlateVisibility::Visible);
	//装上武器后，初始化子弹数量显示到UI上
	if (ADzPlayerCharacter* Player = Cast<ADzPlayerCharacter>(GetOwningPlayerPawn()))
	{
		if (Player->GetHoldWeapon())
		{
			OnWeaponClipChanged(Player->GetHoldWeapon()->GetCurrentClip(), Player->GetHoldWeapon()->GetMaxClipVolume());
			Player->GetHoldWeapon()->OnWeaponClipChanged.AddUObject(this, &ThisClass::OnWeaponClipChanged);
		}
	}
}

void UGameFeatureUserWidget::OnUnEquipWeapon(int32 ID)
{
	WeaponClipTextBlock->SetVisibility(ESlateVisibility::Hidden);
	WeaponClipIcon->SetVisibility(ESlateVisibility::Hidden);
}

void UGameFeatureUserWidget::OnWeaponClipChanged(int32 Current, int32 Max)
{
	WeaponClipTextBlock->SetText(FText::Format(NSLOCTEXT("ui", "Clip", "{0}/{1}"), FText::AsNumber(Current), FText::AsNumber(Max)));
}

void UGameFeatureUserWidget::OnPlayerHPChanged(int32 Current, int32 Max)
{
	HealthBall->UpdateHealth(Current, Max);
}

void UGameFeatureUserWidget::SetGamePlayerPawn(ADzPlayerCharacter* Player)
{
	if (Player && Player->GetPackageComponent())
	{
		Player->GetPackageComponent()->OnEquipWeapon.AddUObject(this, &ThisClass::OnEquipWeapon);
		Player->GetPackageComponent()->OnUnEquipWeapon.AddUObject(this, &ThisClass::OnUnEquipWeapon);

		Player->OnHPChanged.AddUObject(this, &ThisClass::OnPlayerHPChanged);
		OnPlayerHPChanged(Player->GetMaxHP(), Player->GetMaxHP());
		
		WeaponClipTextBlock->SetVisibility(ESlateVisibility::Hidden);
		WeaponClipIcon->SetVisibility(ESlateVisibility::Hidden);
	}
}
