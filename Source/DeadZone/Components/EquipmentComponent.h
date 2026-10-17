// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"


enum class EEquipmentType : uint8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEADZONE_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEquipmentComponent();
	
	void OnPutOnEquipment(EEquipmentType EquipmentType, int32 ID);
	void OnTakeoffEquipment(EEquipmentType EquipmentType, int32 ID);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	TObjectPtr<UStaticMeshComponent> GetStaticMeshComponent(EEquipmentType EquipmentType);
	
	FName GetEquipmentSocketName(EEquipmentType EquipmentType);
	
	TObjectPtr<USkeletalMeshComponent> GetSkeletalMeshComponent();
	
	

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	UPROPERTY()
	TMap<EEquipmentType, TObjectPtr<UStaticMeshComponent>> StaticMeshComponentMap;
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
};
