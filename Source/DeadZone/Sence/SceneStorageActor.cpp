// Fill out your copyright notice in the Description page of Project Settings.


#include "SceneStorageActor.h"

#include "Components/WidgetComponent.h"
#include "DeadZone/Core/DzGameDefines.h"
#include "DeadZone/Subsystem/PropsSubsystem.h"
#include "DeadZone/UI/Game/PromptWidget.h"


// Sets default values
ASceneStorageActor::ASceneStorageActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}


// Called when the game starts or when spawned
void ASceneStorageActor::BeginPlay()
{
	Super::BeginPlay();
	initMesh();
	StaticMeshComponent->SetCollisionProfileName(TEXT("SceneStorageActor"));
	
	if (UPromptWidget* Widget = Cast<UPromptWidget>(PromptWidget->GetWidget()))
	{
		Widget->SetPromptText(FText::FromString(TEXT("打 开")));
	}
}

void ASceneStorageActor::initMesh()
{
	if (ID == -1)
	{
		return;
	}
	const FStorageBase* StorageBase = GetWorld()->GetGameInstance()->GetSubsystem<UPropsSubsystem>()->GetStorage(ID);
	if (!StorageBase)
	{
		return;
	}
	if (StorageBase->StaticMesh)
	{
		GetStaticMeshComponent()->SetStaticMesh(StorageBase->StaticMesh);
	}
	else
	{
		GetSkeletalMeshComponent()->SetSkeletalMesh(StorageBase->SkeletalMesh);
	}
}

void ASceneStorageActor::ChangePrompt(bool IsStorage)
{
	if (UPromptWidget* Widget = Cast<UPromptWidget>(PromptWidget->GetWidget()))
	{
		if (!IsStorage)
		{
			Widget->SetPromptText(FText::FromString(TEXT("打开")));
		} else
		{
			Widget->SetPromptText(FText::FromString(TEXT("关闭")));
		}
		
	}
}

