// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FindPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DeadZone/Character/DzCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

UBTService_FindPlayer::UBTService_FindPlayer()
{
	NodeName = TEXT("Update Player Location");

	// 设置服务执行的默认时间间隔（比如每 0.5 秒更新一次，避免每帧执行浪费性能）
	Interval = 0.5f;
	RandomDeviation = 0.1f; // 随机偏差，防止大量 AI 挤在同一帧计算
}

void UBTService_FindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return;
	}

	// 获取索引为 0 的玩家角色（单人游戏或本地玩家）
	ADzCharacterBase* Target = Cast<ADzCharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	if (Target)
	{
		// 方式 A：如果黑板键类型是 Object，直接把玩家指针存进去（推荐！MoveTo 节点可以直接追逐移动的玩家）
		BlackboardComp->SetValueAsObject(TargetKey.SelectedKeyName, Target);
		
		// 方式 B：如果你的黑板键类型是 Vector，可以取消下面这行注释来存坐标
		// BlackboardComp->SetValueAsVector(PlayerKey.SelectedKeyName, PlayerCharacter->GetActorLocation());
	}
	else
	{
		// 如果玩家死亡或不存在，清空黑板键
		BlackboardComp->ClearValue(TargetKey.SelectedKeyName);
	}
}
