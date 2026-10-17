// Fill out your copyright notice in the Description page of Project Settings.


#include "SceneItemActor.h"

#include "Components/WidgetComponent.h"
#include "DeadZone/Core/DzGameDefines.h"
#include "DeadZone/Subsystem/PropsSubsystem.h"
#include "DeadZone/UI/Game/PromptWidget.h"


// Sets default values
ASceneItemActor::ASceneItemActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASceneItemActor::BeginPlay()
{
	Super::BeginPlay();
	InitMesh();
	if (UPromptWidget* Widget = Cast<UPromptWidget>(PromptWidget->GetWidget()))
	{
		if (ID<CONSUMABLE_INDEX)
		{
			Widget->SetPromptText(FText::FromString(TEXT("装 备")));
		} else
		{
			Widget->SetPromptText(FText::FromString(TEXT("拾 取")));
		}
		
	}
}


void ASceneItemActor::InitMesh()
{
	if (ID == -1)
	{
		return;
	}
	//去数据管理器中产找ID是否存在
	const FPropsBase* PropsBase = GetWorld()->GetGameInstance()->GetSubsystem<UPropsSubsystem>()->GetProps(ID);
	if (!PropsBase)
	{
		return;
	}
	//检查当前获取到的数据是什么类型
	if (PropsBase->PropsType == EPropsType::EPT_Equipment)
	{
		const FEquipmentHeader* EquipmentHeader = static_cast<const FEquipmentHeader*>(PropsBase);
		if (EquipmentHeader->StaticMesh)
		{
			GetStaticMeshComponent()->SetStaticMesh(EquipmentHeader->StaticMesh);
		}
		else
		{
			GetSkeletalMeshComponent()->SetSkeletalMesh(EquipmentHeader->SkeletalMesh);
		}
	}
	else if (PropsBase->PropsType == EPropsType::EPT_Weapon)
	{
		const FWeaponHeader* WeaponHeader = static_cast<const FWeaponHeader*>(PropsBase);
		GetSkeletalMeshComponent()->SetSkeletalMesh(WeaponHeader->SkeletalMesh);
	}
	else
	{
		const FConsumableHeader* ConsumableHeader = static_cast<const FConsumableHeader*>(PropsBase);
		if (ConsumableHeader->StaticMesh)
		{
			GetStaticMeshComponent()->SetStaticMesh(ConsumableHeader->StaticMesh);
		}
		else
		{
			GetSkeletalMeshComponent()->SetSkeletalMesh(ConsumableHeader->SkeletalMesh);
		}
	}
}

