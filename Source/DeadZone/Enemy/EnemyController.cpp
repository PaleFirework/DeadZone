// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"

#include "EnemyCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DeadZone/Core/DzGameDefines.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"


// Sets default values
AEnemyController::AEnemyController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::OnPerceptionTargetUpdate);
}

// Called when the game starts or when spawned
void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(InPawn))
	{
		//启动行为树
		RunBehaviorTree(EnemyCharacter->GetBehaviorTree());
		
		GetBlackboardComponent()->SetValueAsVector(TEXT("BirthPos"), InPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("NavPosition"), InPawn->GetActorLocation() + InPawn->GetActorForwardVector() * 1000);
		
		if (EnemyCharacter->GetTeamType() == ETeamType::ETT_Player)
		{
			GetBlackboardComponent()->SetValueAsBool(TEXT("bFriend"), true);
		}
		EnemyCharacter->OnAttacked.AddUObject(this, &ThisClass::HandleOnAttacked);
	}
}

void AEnemyController::OnPerceptionTargetUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())//检查发生的感知类型是否是听觉
	{
		//更新到黑板中的可疑点
		GetBlackboardComponent()->SetValueAsVector(TEXT("SuspiciousPos"), Stimulus.StimulusLocation);
	}else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		//视觉
		if (Stimulus.WasSuccessfullySensed())//如果返回真，则表明看到了！
		{
			AActor* oldTarget = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
			if (!oldTarget)
			{
				GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Actor);
			}
			else if (oldTarget != Actor)
			{
				//比谁离我更近
				if ((Actor->GetActorLocation() - GetPawn()->GetActorLocation()).Size() <
					(oldTarget->GetActorLocation() - GetPawn()->GetActorLocation()).Size())
				{
					GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Actor);
				}
			}
		}
	}
}

ETeamAttitude::Type AEnemyController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (ADzCharacterBase* MyDzCharacter = Cast<ADzCharacterBase>(GetPawn()))
	{
		if (const ADzCharacterBase* OtherCharacter = Cast<const ADzCharacterBase>(&Other))
		{
			return OtherCharacter->GetTeamType() == MyDzCharacter->GetTeamType() ? ETeamAttitude::Friendly : ETeamAttitude::Hostile;
		}
	}
	return ETeamAttitude::Hostile;
}

void AEnemyController::HandleOnAttacked(AController* AttackerController)
{
	if (!AttackerController || !AttackerController->GetPawn() || !GetPawn()) return;

	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB) return;

	APawn* NewAttackerPawn = AttackerController->GetPawn();
	AActor* CurrentTarget = Cast<AActor>(BB->GetValueAsObject(TEXT("Target")));

	if (!CurrentTarget)
	{
		BB->SetValueAsObject(TEXT("Target"), NewAttackerPawn);
	}
	else if (CurrentTarget != NewAttackerPawn)
	{
		const FVector SelfLoc = GetPawn()->GetActorLocation();
		const float DistToNew = FVector::DistSquared(SelfLoc, NewAttackerPawn->GetActorLocation());
		const float DistToOld = FVector::DistSquared(SelfLoc, CurrentTarget->GetActorLocation());
		
		if (DistToNew < DistToOld)
		{
			BB->SetValueAsObject(TEXT("Target"), NewAttackerPawn);
		}
	}
}

// Called every frame
void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyController::NotifyDead()
{
	StopMovement();
	if (UBehaviorTreeComponent* BehaviorTree = Cast<UBehaviorTreeComponent>(BrainComponent))
	{
		BehaviorTree->StopTree();
	}
	//停止感知组件
	if (PerceptionComponent)
	{
		PerceptionComponent->OnTargetPerceptionUpdated.Clear();//清理通知
		PerceptionComponent->SetActive(false);//解除组件激活
	}
	if (ADzCharacterBase* DzChar = Cast<ADzCharacterBase>(GetPawn()))
	{
		DzChar->OnAttacked.RemoveAll(this);
	}
	UnPossess();//解除对于角色的控制
	//SetLifeSpan(5.f);
}

