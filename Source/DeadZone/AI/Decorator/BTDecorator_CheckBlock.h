// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckBlock.generated.h"

/**
 * 
 */
UCLASS()
class DEADZONE_API UBTDecorator_CheckBlock : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	
	UBTDecorator_CheckBlock();
	
	
protected:
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	virtual uint16 GetInstanceMemorySize() const override;
	
protected:
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector APointerKey;
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector BPointerKey;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ETraceTypeQuery> TraceChannel;
	UPROPERTY(EditAnywhere)
	float OffsetAPointer;
	UPROPERTY(EditAnywhere)
	float OffsetBPointer;
	UPROPERTY(EditAnywhere)
	float CheckInterval = 0.5f;
	
};
