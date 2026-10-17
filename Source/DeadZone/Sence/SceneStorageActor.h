// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SceneBaseActor.h"
#include "DeadZone/Components/PackageComponent.h"
#include "GameFramework/Actor.h"
#include "SceneStorageActor.generated.h"


class UWidgetComponent;



UCLASS()
class DEADZONE_API ASceneStorageActor : public ASceneBaseActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASceneStorageActor();
	void initMesh();
	void ChangePrompt(bool IsStorage);
	TMap<int32, FInventorySlot>& GetItems() {return Items;};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditAnywhere)
	int32 StorageMax;
	UPROPERTY(EditAnywhere)
	TMap<int32, FInventorySlot> Items;
};
