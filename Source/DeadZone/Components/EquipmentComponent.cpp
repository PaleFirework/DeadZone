// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentComponent.h"

#include "DeadZone/Interface/EquipmentInterface.h"
#include "DeadZone/Subsystem/PropsSubsystem.h"


class UPropsSubsystem;
struct FPropsBase;
// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipmentComponent::OnPutOnEquipment(EEquipmentType EquipmentType, int32 ID)
{
	if (const FPropsBase* PropsBase = GetWorld()->GetGameInstance()->GetSubsystem<UPropsSubsystem>()->GetProps(ID))
	{
		if (PropsBase->PropsType == EPropsType::EPT_Weapon)
		{
			return;
		}
		
		const FEquipmentHeader* EquipmentHeader = static_cast<const FEquipmentHeader*>(PropsBase);
		
		if (EquipmentType == EEquipmentType::EST_Clothing)//骨架网格资产
		{
			GetSkeletalMeshComponent()->SetSkeletalMesh(EquipmentHeader->SkeletalMesh);
		}
		else
		{
			GetStaticMeshComponent(EquipmentType)->SetStaticMesh(EquipmentHeader->StaticMesh);
		}
	}
}

void UEquipmentComponent::OnTakeoffEquipment(EEquipmentType EquipmentType, int32 ID)
{
	if (EquipmentType == EEquipmentType::EST_Clothing)
	{
		GetSkeletalMeshComponent()->SetSkeletalMesh(nullptr);
	}
	else
	{
		GetStaticMeshComponent(EquipmentType)->SetStaticMesh(nullptr);
	}
}


// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

TObjectPtr<UStaticMeshComponent> UEquipmentComponent::GetStaticMeshComponent(EEquipmentType EquipmentType)
{
	if (StaticMeshComponentMap.Contains(EquipmentType))
	{
		return StaticMeshComponentMap[EquipmentType];
	}
	UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(GetOwner());
	StaticMeshComponent->RegisterComponentWithWorld(GetWorld());
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	//设置层级关系
	if (IEquipmentInterface* Character = Cast<IEquipmentInterface>(GetOwner()))
	{ 
		StaticMeshComponent->AttachToComponent(Character->GetSkeletalMeshComponent(), 
			FAttachmentTransformRules::SnapToTargetNotIncludingScale, GetEquipmentSocketName(EquipmentType));
	}
	StaticMeshComponentMap.Add(EquipmentType, StaticMeshComponent);
	return StaticMeshComponent;
}

FName UEquipmentComponent::GetEquipmentSocketName(EEquipmentType EquipmentType)
{
	FName SocketName = NAME_None;
	if (EquipmentType == EEquipmentType::EST_Package)
	{
		SocketName = TEXT("PackageSocket");
	}
	else
	{
		SocketName = TEXT("HeadSocket");
	}
	
	return SocketName;
}

TObjectPtr<USkeletalMeshComponent> UEquipmentComponent::GetSkeletalMeshComponent()
{
	if (!SkeletalMeshComponent)
	{
		SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(GetOwner());
		SkeletalMeshComponent->RegisterComponentWithWorld(GetWorld());
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (IEquipmentInterface* Character = Cast<IEquipmentInterface>(GetOwner()))
		{
			SkeletalMeshComponent->AttachToComponent(Character->GetSkeletalMeshComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			//设置动作Leader
			SkeletalMeshComponent->SetLeaderPoseComponent(Character->GetSkeletalMeshComponent());
		}
	}
	return SkeletalMeshComponent;
}


// Called every frame
void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

