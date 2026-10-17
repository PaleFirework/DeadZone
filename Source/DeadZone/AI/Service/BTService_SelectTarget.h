// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SelectTarget.generated.h"

/**
 * 
 */
UCLASS()
class DEADZONE_API UBTService_SelectTarget : public UBTService
{
	GENERATED_BODY()
	
	UBTService_SelectTarget();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	
protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
