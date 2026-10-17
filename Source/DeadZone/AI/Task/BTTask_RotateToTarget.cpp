// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RotateToTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DeadZone/Zombie/ZombieCharacter.h"

UBTTask_RotateToTarget::UBTTask_RotateToTarget()
{
	NodeName = TEXT("Rotate To Target");
	
	bNotifyTick = true;
}


EBTNodeResult::Type UBTTask_RotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	AZombieCharacter* Zombie =
		Cast<AZombieCharacter>(AIController->GetPawn());

	if (!Zombie)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComp =
		OwnerComp.GetBlackboardComponent();

	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	AActor* Target =
		Cast<AActor>(
			BlackboardComp->GetValueAsObject(
				TargetKey.SelectedKeyName
			)
		);

	if (!Target)
	{
		return EBTNodeResult::Failed;
	}

	// --------------------------------
	// 计算目标方向
	// --------------------------------

	const FVector Direction =
		Target->GetActorLocation() -
		Zombie->GetActorLocation();

	FRotator TargetRotation = Direction.Rotation();

	TargetRotation.Pitch = 0.f;
	TargetRotation.Roll = 0.f;

	// --------------------------------
	// 计算当前与目标之间的最短角度
	// --------------------------------

	const float Angle =
		FMath::FindDeltaAngleDegrees(
			Zombie->GetActorRotation().Yaw,
			TargetRotation.Yaw
		);

	// 已经基本朝向目标
	if (FMath::Abs(Angle) <= 2.f)
	{
		Zombie->SetActorRotation(TargetRotation);

		return EBTNodeResult::Succeeded;
	}

	// --------------------------------
	// 播放对应的转身动画
	// --------------------------------

	UAnimMontage* Montage = Zombie->PlayTurnMontage(Angle);

	if (!Montage)
	{
		return EBTNodeResult::Failed;
	}

	// Task 必须等待 Montage 播放完成
	return EBTNodeResult::InProgress;
	
}

void UBTTask_RotateToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController =
		OwnerComp.GetAIOwner();

	if (!AIController)
	{
		FinishLatentTask(
			OwnerComp,
			EBTNodeResult::Failed
		);

		return;
	}

	AZombieCharacter* Zombie =
		Cast<AZombieCharacter>(
			AIController->GetPawn()
		);

	if (!Zombie)
	{
		FinishLatentTask(
			OwnerComp,
			EBTNodeResult::Failed
		);

		return;
	}

	UBlackboardComponent* BlackboardComp =
		OwnerComp.GetBlackboardComponent();

	if (!BlackboardComp)
	{
		FinishLatentTask(
			OwnerComp,
			EBTNodeResult::Failed
		);

		return;
	}

	AActor* Target =
		Cast<AActor>(
			BlackboardComp->GetValueAsObject(
				TargetKey.SelectedKeyName
			)
		);

	if (!Target)
	{
		FinishLatentTask(
			OwnerComp,
			EBTNodeResult::Failed
		);

		return;
	}

	// --------------------------------
	// 重新计算目标方向
	// --------------------------------

	const FVector Direction =
		Target->GetActorLocation() -
		Zombie->GetActorLocation();

	FRotator TargetRotation =
		Direction.Rotation();

	TargetRotation.Pitch = 0.f;
	TargetRotation.Roll = 0.f;

	// --------------------------------
	// 平滑修正 Actor Rotation
	// --------------------------------

	/*const FRotator NewRotation =
		FMath::RInterpConstantTo(
			Zombie->GetActorRotation(),
			TargetRotation,
			DeltaSeconds,
			360.f
		);

	Zombie->SetActorRotation(NewRotation);*/

	// --------------------------------
	// 检查 Montage 是否播放结束
	// --------------------------------

	UAnimInstance* AnimInstance =
		Zombie->GetMesh()->GetAnimInstance();

	if (!AnimInstance)
	{
		FinishLatentTask(
			OwnerComp,
			EBTNodeResult::Failed
		);

		return;
	}

	// 如果当前已经没有 Montage 在播放
	if (!AnimInstance->IsAnyMontagePlaying())
	{
		// 最后强制修正到准确方向
		Zombie->SetActorRotation(TargetRotation);

		FinishLatentTask(
			OwnerComp,
			EBTNodeResult::Succeeded
		);
	}
}

uint16 UBTTask_RotateToTarget::GetInstanceMemorySize() const
{
	return 0;
}
