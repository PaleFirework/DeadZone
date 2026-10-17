// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerModelActor.h"

#include "Components/SceneCaptureComponent2D.h"
#include "DeadZone/Components/EquipmentComponent.h"
#include "DeadZone/Components/PackageComponent.h"
#include "DeadZone/Player/DzPlayerCharacter.h"


// Sets default values
APlayerModelActor::APlayerModelActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	SceneCaptureComponent->SetupAttachment(RootComponent);
	
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));
}

void APlayerModelActor::SetPlayer(ADzPlayerCharacter* Player)
{
	if (Player && Player->GetPackageComponent())
	{
		Player->GetPackageComponent()->OnPutOnEquipment.AddUObject(EquipmentComponent, &UEquipmentComponent::OnPutOnEquipment);
		Player->GetPackageComponent()->OnTakeoffEquipment.AddUObject(EquipmentComponent, &UEquipmentComponent::OnTakeoffEquipment);
		SkeletalMeshComponent->SetSkeletalMesh(Player->GetMesh()->GetSkeletalMeshAsset());
	}
}

// Called when the game starts or when spawned
void APlayerModelActor::BeginPlay()
{
	Super::BeginPlay();
	
}

USkeletalMeshComponent* APlayerModelActor::GetSkeletalMeshComponent()
{
	return SkeletalMeshComponent;
}

// Called every frame
void APlayerModelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

