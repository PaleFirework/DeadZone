// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Sprint.h"

#include "AIController.h"
#include "DeadZone/Character/DzCharacterBase.h"

EBTNodeResult::Type UBTTask_Sprint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ADzCharacterBase* CharacterBase = Cast<ADzCharacterBase>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		if (bSprint)
		{
			CharacterBase->StartSprint();
		}
		else
		{
			CharacterBase->StopSprint();
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
