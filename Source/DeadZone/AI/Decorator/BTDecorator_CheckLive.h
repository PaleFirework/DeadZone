// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckLive.generated.h"

/**
 * 
 */
UCLASS()
class DEADZONE_API UBTDecorator_CheckLive : public UBTDecorator
{
	GENERATED_BODY()
	
protected:
	
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
public:
	
	UBTDecorator_CheckLive();
	
	
protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetKey;
	
};
