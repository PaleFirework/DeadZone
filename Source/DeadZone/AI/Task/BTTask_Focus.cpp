// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Focus.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTTask_Focus::UBTTask_Focus()
{
	FocusTarget.AddObjectFilter(this, TEXT("BlackboardKeySelector"), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_Focus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (FocusTarget.IsNone())
	{
		return EBTNodeResult::Failed;
	}
	if (AActor* Actor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(FocusTarget.GetSelectedKeyID())))
	{
		//如果想要使用此方法需要保证AI控制的角色开启了busercontrolleryaw
		if (bFocus)
		{
			OwnerComp.GetAIOwner()->SetFocus(Actor);
		}
		else
		{
			OwnerComp.GetAIOwner()->ClearFocus(EAIFocusPriority::Gameplay);
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

void UBTTask_Focus::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BBData = GetBlackboardAsset();
	if (BBData)
	{
		FocusTarget.ResolveSelectedKey(*BBData);
	}
}

