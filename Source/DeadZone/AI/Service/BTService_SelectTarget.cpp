// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SelectTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DeadZone/Character/DzCharacterBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

UBTService_SelectTarget::UBTService_SelectTarget()
{
	NodeName = TEXT("选择目标");
	bCreateNodeInstance = true;
}

void UBTService_SelectTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AIC = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AIC || !BB || !AIC->GetPawn()) return;

	UAIPerceptionComponent* PerceptionComp = AIC->GetAIPerceptionComponent();
	if (!PerceptionComp) return;

	TArray<AActor*> PerceivedActors;
	PerceptionComp->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);

	AActor* BestTarget = nullptr;
	float MinDistanceSq = FLT_MAX;
	const FVector AILocation = AIC->GetPawn()->GetActorLocation();

	// 遍历视野内的所有目标，找出最近且存活的目标
	for (AActor* Actor : PerceivedActors)
	{
		IDeadInterface* TargetChar = Cast<IDeadInterface>(Actor);
		
		if (TargetChar && !TargetChar->IsDead())
		{
			float DistSq = FVector::DistSquared(AILocation, Actor->GetActorLocation());
			if (DistSq < MinDistanceSq)
			{
				MinDistanceSq = DistSq;
				BestTarget = Actor;
			}
		}
	}
	
	if (BestTarget)
	{
		BB->SetValueAsObject(TEXT("Target"), BestTarget);
	}
	else
	{
		BB->ClearValue(TEXT("Target"));
	}
}


