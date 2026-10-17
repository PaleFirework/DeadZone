// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckBlock.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTDecorator_CheckBlock::UBTDecorator_CheckBlock()
{
	APointerKey.AddVectorFilter(this, TEXT("APointerKey"));
	BPointerKey.AddVectorFilter(this, TEXT("BPointerKey"));
	
	APointerKey.AddObjectFilter(this, TEXT("APointerKey"), AActor::StaticClass());
	BPointerKey.AddObjectFilter(this, TEXT("BPointerKey"), AActor::StaticClass());
	
	bNotifyTick = true;//开启Tick节点调用
}

bool UBTDecorator_CheckBlock::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (APointerKey.IsNone() || BPointerKey.IsNone())//检查Key是否有效
	{
		return false;
	}
	//检查AB之间有没有障碍物
	FVector APointer;
	FVector BPointer;
	
	//设置忽略参数
	FCollisionQueryParams Params;
	
	if (APointerKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		APointer = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(APointerKey.GetSelectedKeyID());
	}
	else
	{
		AActor* Actor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(APointerKey.GetSelectedKeyID()));
		if (!IsValid(Actor))
		{
			return false;
		}
		//增加忽略
		Params.AddIgnoredActor(Actor);
		APointer = Actor->GetActorLocation();
	}
	
	if (BPointerKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		BPointer = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(BPointerKey.GetSelectedKeyID());
	}
	else
	{
		AActor* Actor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(BPointerKey.GetSelectedKeyID()));
		if (!IsValid(Actor))
		{
			return false;
		}
		//增加忽略
		Params.AddIgnoredActor(Actor);
		BPointer = Actor->GetActorLocation();
	}
	
	//检测阻挡 单目标还是多目标可以根据需求来选择
	return OwnerComp.GetAIOwner()->GetWorld()->LineTraceTestByChannel(APointer + FVector::UpVector * OffsetAPointer, BPointer + FVector::UpVector * OffsetBPointer,
		UEngineTypes::ConvertToCollisionChannel(TraceChannel), Params);
}

void UBTDecorator_CheckBlock::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BBData = GetBlackboardAsset();
	if (BBData)
	{
		APointerKey.ResolveSelectedKey(*BBData);
		BPointerKey.ResolveSelectedKey(*BBData);
	}
}

void UBTDecorator_CheckBlock::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	//不停的检查是否安全,是否有必要每一帧都检查是否安全
	float* pTickTime = reinterpret_cast<float*>(NodeMemory);
	if ( (*pTickTime -= DeltaSeconds) <= 0.0f)
	{
		//执行检查是否有阻挡
		if (!CalculateRawConditionValue(OwnerComp, NodeMemory))//如果有阻挡，我们不需要打断休息，如果没有阻挡，则需要终止任务，刷新执行权
		{
			//ConditionPassing 立刻放下手头的工作，重新刷新执行权
			//ConditionResultChanged 等先忙完手里的这个活，再刷新执行
			//例如一堆碗需要清晰，Passing立刻停下不要洗了，Result先洗完手里的那个碗，剩下的不洗了
			ConditionalFlowAbort(OwnerComp, EBTDecoratorAbortRequest::ConditionPassing);
		}
		*pTickTime = CheckInterval;
	}
}

uint16 UBTDecorator_CheckBlock::GetInstanceMemorySize() const
{
	return sizeof(float);
}
