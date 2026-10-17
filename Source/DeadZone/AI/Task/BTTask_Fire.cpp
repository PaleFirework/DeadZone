// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Fire.h"

#include "AIController.h"
#include "DeadZone/Character/DzCharacterBase.h"

EBTNodeResult::Type UBTTask_Fire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ADzCharacterBase* CharacterBase = Cast<ADzCharacterBase>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		if (CharacterBase->GetHoldWeapon())
		{
			if (bFire)
			{
				CharacterBase->StartIronsight();
				CharacterBase->StartFire();
			}
			else
			{
				CharacterBase->StopIronsight();
			}
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
