// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ClearValue.generated.h"

/**
 * 
 */
UCLASS()
class DEADZONE_API UBTTask_ClearValue : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector BlackboardKeySelector;
};
