// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SceneBaseActor.generated.h"

class UWidgetComponent;

UCLASS()
class DEADZONE_API ASceneBaseActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASceneBaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	TObjectPtr<UStaticMeshComponent> GetStaticMeshComponent();
	TObjectPtr<USkeletalMeshComponent> GetSkeletalMeshComponent();

public:
	// Called every frame
	void SetItemOutline(bool bEnable);
	void SetItemPrompt(bool bEnable);
	int32 GetID() const { return ID; }
	void SetID(int32 NewID) { ID = NewID; }
	
protected:
	UPROPERTY(EditAnywhere)
	int32 ID = -1;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBillboardComponent> BillboardComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> PromptWidget;
};
