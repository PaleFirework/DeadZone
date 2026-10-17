// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeadZone/Interface/EquipmentInterface.h"
#include "GameFramework/Actor.h"
#include "PlayerModelActor.generated.h"

class ADzPlayerCharacter;
class UEquipmentComponent;

UCLASS()
class DEADZONE_API APlayerModelActor : public AActor, public IEquipmentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerModelActor();
	
	void SetPlayer(ADzPlayerCharacter* Player);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual USkeletalMeshComponent* GetSkeletalMeshComponent() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneCaptureComponent2D> SceneCaptureComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UEquipmentComponent> EquipmentComponent;
};
