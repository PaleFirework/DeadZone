// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindNavPosition.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTTask_FindNavPosition::UBTTask_FindNavPosition()
{
	NodeName = TEXT("随机寻找导航点");
	
	//添加黑板拾取器的类型约束
	OriginPosKey.AddVectorFilter(this, TEXT("OriginPosKey"));
	OriginPosKey.AddObjectFilter(this, TEXT("OriginPosKey"), AActor::StaticClass());
	
	NavPosKey.AddVectorFilter(this, TEXT("NavPosKey"));
	GoLeftKey.AddBoolFilter(this, TEXT("GoLeft"));
}

EBTNodeResult::Type UBTTask_FindNavPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//检查键值拾取器是否有进行拾取操作数据
	if (OriginPosKey.IsNone() || NavPosKey.IsNone())
	{
		return EBTNodeResult::Failed;
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GoLeftKey.SelectedKeyName,FMath::RandBool());
	//先获取源点坐标
	FVector OriginPos;
	//判断用户使用键值拾取器拾取的数据类型
	if (OriginPosKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		OriginPos = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(OriginPosKey.GetSelectedKeyID());
	}
	else
	{
		AActor* OriginActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(OriginPosKey.GetSelectedKeyID()));
		if (!IsValid(OriginActor))
		{
			return EBTNodeResult::Failed;
		}
		OriginPos = OriginActor->GetActorLocation();
	}
	FVector NavPos;
	//随机找导航覆盖的点
	if (UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(OwnerComp.GetAIOwner(), OriginPos, NavPos, RandomRadius))
	{
		//更新数据到黑板
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(NavPosKey.GetSelectedKeyID(), NavPos);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}

void UBTTask_FindNavPosition::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	if (UBlackboardData* BbData = GetBlackboardAsset())
	{
		OriginPosKey.ResolveSelectedKey(*BbData);//让黑板键值拾取和黑板数据进行关联
		NavPosKey.ResolveSelectedKey(*BbData);
	}
}
