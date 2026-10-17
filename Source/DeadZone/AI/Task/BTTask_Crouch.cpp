// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Crouch.h"

#include "AIController.h"
#include "DeadZone/Character/DzCharacterBase.h"

EBTNodeResult::Type UBTTask_Crouch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ADzCharacterBase* CharacterBase = Cast<ADzCharacterBase>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		if (bCrouch != CharacterBase->IsCrouch())
		{
			CharacterBase->DoCrouch();
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
