// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckLive.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "DeadZone/Character/DzCharacterBase.h"
#include "DeadZone/Zombie/ZombieCharacter.h"


UBTDecorator_CheckLive::UBTDecorator_CheckLive()
{
	TargetKey.AddObjectFilter(this, TEXT("TargetKey"), ADzCharacterBase::StaticClass());
	bNotifyTick = true;
}


void UBTDecorator_CheckLive::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	
	UBlackboardData* BBData = GetBlackboardAsset();
	if (BBData)
	{
		TargetKey.ResolveSelectedKey(*BBData);
	}
}

bool UBTDecorator_CheckLive::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (TargetKey.IsNone())
	{
		return false;
	}
	if (ADzCharacterBase* CharacterBase = Cast<ADzCharacterBase>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(TargetKey.GetSelectedKeyID())))
	{
		return !CharacterBase->IsDead();
	}
	if (AZombieCharacter* CharacterBase = Cast<AZombieCharacter>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(TargetKey.GetSelectedKeyID())))
	{
		return !CharacterBase->IsDead();
	}
	return false;
}

void UBTDecorator_CheckLive::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//如果发生了目标死亡，如果目标死亡我们需要终止任务
	if (!CalculateRawConditionValue(OwnerComp, NodeMemory))
	{
		//观察者终止
		// ConditionalFlowAbort(OwnerComp, EBTDecoratorAbortRequest::ConditionPassing);//强制行为重新计算执行线
		// ConditionalFlowAbort(OwnerComp, EBTDecoratorAbortRequest::ConditionResultChanged);//检查装饰器结果是否发生了变化，如果变化才会刷新执行线
		OwnerComp.RequestExecution(this);//让行为树组件重新执行下当前挂载的装饰器节点
	}
}
