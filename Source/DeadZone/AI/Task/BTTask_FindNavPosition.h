// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindNavPosition.generated.h"

/**
 * 
 */
UCLASS()
class DEADZONE_API UBTTask_FindNavPosition : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_FindNavPosition();
	
protected:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
protected:
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector OriginPosKey;
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector NavPosKey;
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector GoLeftKey;
	UPROPERTY(EditAnywhere)
	float RandomRadius = 1500;
	
};
