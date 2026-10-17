// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SceneBaseActor.h"
#include "GameFramework/Actor.h"
#include "SceneItemActor.generated.h"

class UWidgetComponent;

UCLASS()
class DEADZONE_API ASceneItemActor : public ASceneBaseActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASceneItemActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void InitMesh();
	bool GetIsDrag() const { return bIsDrag; }
	void SetIsDrag(bool bNewIsDrag) { bIsDrag = bNewIsDrag; }
	int GetAmount(){return Amount;};
	void SetAmount(int NewAmount) { Amount = NewAmount; }

protected:
	UPROPERTY()
	bool bIsDrag = false;
	UPROPERTY(EditAnywhere)
	int32 Amount = 1;
};
