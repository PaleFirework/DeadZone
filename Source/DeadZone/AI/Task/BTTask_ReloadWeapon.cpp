// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ReloadWeapon.h"

#include "AIController.h"
#include "DeadZone/Character/DzCharacterBase.h"
#include "DeadZone/Weapon/WeaponActor.h"

UBTTask_ReloadWeapon::UBTTask_ReloadWeapon()
{
	bNotifyTick = true;//开启Tick用来检查是否换弹结束
}

EBTNodeResult::Type UBTTask_ReloadWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ADzCharacterBase* CharacterBase = Cast<ADzCharacterBase>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		if (CharacterBase->GetHoldWeapon())
		{
			CharacterBase->ReloadWeapon();
			return EBTNodeResult::InProgress;
		}
	}
	return EBTNodeResult::Failed;
}

void UBTTask_ReloadWeapon::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//检查角色的换弹是否结束
	if (ADzCharacterBase* CharacterBase = Cast<ADzCharacterBase>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		if (CharacterBase->GetHoldWeapon() && CharacterBase->GetHoldWeapon()->GetCurrentState() == EWeaponState::EWS_Normal)
		{
			//让当前的行为树的任务节点结束占用执行权
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}
