// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearValue.h"

#include "BehaviorTree/BlackboardComponent.h"

void UBTTask_ClearValue::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BBData = GetBlackboardAsset();
	if (BBData)
	{
		BlackboardKeySelector.ResolveSelectedKey(*BBData);
	}
}

EBTNodeResult::Type UBTTask_ClearValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (BlackboardKeySelector.IsNone())
	{
		return EBTNodeResult::Failed;
	}
	//清理黑板中的数据
	OwnerComp.GetBlackboardComponent()->ClearValue(BlackboardKeySelector.GetSelectedKeyID());
	return EBTNodeResult::Succeeded;
}
