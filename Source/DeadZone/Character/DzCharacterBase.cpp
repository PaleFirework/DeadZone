// Fill out your copyright notice in the Description page of Project Settings.


#include "DzCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "DeadZone/Components/DzCharacterMovementComponent.h"
#include "DeadZone/Components/EquipmentComponent.h"
#include "DeadZone/Components/PackageComponent.h"
#include "DeadZone/GamePlay/Game/DzGameMode.h"
#include "DeadZone/GamePlay/Game/DzPlayerState.h"
#include "DeadZone/Player/DzPlayerCharacter.h"
#include "DeadZone/Weapon/WeaponActor.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"


// Sets default values
ADzCharacterBase::ADzCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UDzCharacterMovementComponent>(CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	PackageComponent = CreateDefaultSubobject<UPackageComponent>(TEXT("PackageComponent"));
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));
	
	/*BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComponent"));
	BillboardComponent->SetupAttachment(RootComponent);
	BillboardComponent->SetRelativeLocation(FVector(0, 0, 110.f));*/
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(ReplicationMode);
}

// Called when the game starts or when spawned
void ADzCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	if (PackageComponent && EquipmentComponent)
	{
		//绑定换装通知
		PackageComponent->OnPutOnEquipment.AddUObject(EquipmentComponent, &UEquipmentComponent::OnPutOnEquipment);
		PackageComponent->OnTakeoffEquipment.AddUObject(EquipmentComponent, &UEquipmentComponent::OnTakeoffEquipment);
		
		PackageComponent->OnEquipWeapon.AddUObject(this, &ADzCharacterBase::OnEquipWeapon);
		PackageComponent->OnUnEquipWeapon.AddUObject(this, &ADzCharacterBase::OnUnEquipWeapon);
	}
	CurHP = MaxHP;
}

// Called every frame
void ADzCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADzCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

TObjectPtr<AWeaponActor> ADzCharacterBase::GetHoldWeapon() const
{
	{
		if (!PackageComponent)
		{
			return nullptr;
		}
		return PackageComponent->GetHoldWeapon();
	}
}

bool ADzCharacterBase::IsSprinting() const
{
	bool bCanSprite = bSprint && !bIsCrouched;//蹲下去的时候不能有冲刺

	if (GetHoldWeapon())//持枪只能朝前的冲刺
	{
		bCanSprite = bCanSprite && FVector::DotProduct(GetVelocity().GetSafeNormal2D(), GetActorForwardVector()) > 0.9f;
	}
	
	return bCanSprite;
}

bool ADzCharacterBase::IsInAir() const
{
	if (const UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		return MovementComp->IsFalling();
	}

	return false;
}

bool ADzCharacterBase::IsIronsight() const
{
	return bIronsight;
}

bool ADzCharacterBase::IsDead() const
{
	return CurHP <= 0;
}

bool ADzCharacterBase::IsCrouch() const
{
	return bCrouch;
}

void ADzCharacterBase::Jump()
{
	Super::Jump();
	if (GetHoldWeapon())
	{
		StopIronsight();
	}
}

void ADzCharacterBase::StartSprint()
{
	bSprint = true;
	if (GetHoldWeapon())
	{
		StopIronsight();
	}
}

void ADzCharacterBase::StopSprint()
{
	bSprint = false;
}

void ADzCharacterBase::StartFire()
{
	if (IsIronsight() && GetHoldWeapon())
	{
		GetHoldWeapon()->StartFire();
		if (Cast<ADzPlayerCharacter>(this))
		{
			UAISense_Hearing::ReportNoiseEvent(
			GetWorld(),                // 上下文 World
			GetActorLocation(),        // 发声位置（也可以用武器枪口 Socket 位置）
			1.0f,                      // 响度 (Loudness)，默认 1.0
			this,                      // 发声者 Instigator (非常重要！不能为 nullptr)
			5000.0f,                   // 声音传播的最大距离 (MaxRange，单位: 厘米)
			FName("Gunshot")           // 声音标签 Tag (可选，方便 AI 区分声音类型)
			);
		}
		
	}
}

void ADzCharacterBase::StopFire()
{
	if (GetHoldWeapon())
	{
		GetHoldWeapon()->StopFire();
	}
}

void ADzCharacterBase::StartIronsight()
{
	if (GetHoldWeapon() && !IsInAir())
	{
		bIronsight = true;
		StopSprint();
	}
}

void ADzCharacterBase::StopIronsight()
{
	bIronsight = false;
	StopFire();
}

void ADzCharacterBase::ReloadWeapon()
{
	if (GetHoldWeapon())
	{
		GetHoldWeapon()->ReloadWeapon();
	}
}

ETeamType ADzCharacterBase::GetTeamType() const
{
	return TeamType;
}


void ADzCharacterBase::DoCrouch()
{
	if (CanCrouch())
	{
		Crouch();
		bCrouch = true;
	}
	else
	{
		UnCrouch();
		bCrouch = false;
	}
}

void ADzCharacterBase::OnEquipWeapon(int32 ID)
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void ADzCharacterBase::OnUnEquipWeapon(int32 ID)
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ADzCharacterBase::EquipWeapon(int32 ID)
{
	if (PackageComponent)
	{
		PackageComponent->EquipWeapon(ID);
	}
}

void ADzCharacterBase::OnDead()
{
	StopFire();
	//关闭移动
	GetCharacterMovement()->StopActiveMovement();
	//设置5秒后删除
	//SetLifeSpan(5.f);
}

USkeletalMeshComponent* ADzCharacterBase::GetSkeletalMeshComponent()
{
	return GetMesh();
}

float ADzCharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if (IsDead())
	{
		return 0;
	}
	int32 DamageValue = 0;
	if (OnAttacked.IsBound())
	{
		OnAttacked.Broadcast(EventInstigator);
	}

	const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
	
	if (PointDamageEvent)
	{
		const FHitResult& HitResult = PointDamageEvent->HitInfo;

		const FVector HitLocation = HitResult.ImpactPoint;
		const FRotator HitRotation = HitResult.ImpactNormal.Rotation();

		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HurtEffect,
			HitLocation,
			HitRotation,
			FVector(1.0f),
			true,
			EPSCPoolMethod::None,
			true
		);
	}
	
	//计算伤害
	if (ADzGameMode* Gm = Cast<ADzGameMode>(GetWorld()->GetAuthGameMode()))
	{
		DamageValue = Gm->GetDamageValueWithBoneName(PointDamageEvent->HitInfo.BoneName);
		if (Cast<ADzPlayerCharacter>(this))
		{
			DamageValue *= 0.5f;
		}
		CurHP -= DamageValue;
		if (CurHP < 0)
		{
			CurHP = 0;
		}
	}
	//开启布娃娃效果
	if (CurHP<= 0)
	{
		float Force = FMath::Clamp(10000 - (DamageCauser->GetActorLocation() - GetActorLocation()).Size(), 0, 10000) / 10000.0f * 5000.f + 2000.f;
		
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);//这一步至关重要，这个设置也可以防止枪械鞭尸
		GetMesh()->SetSimulatePhysics(true);
		
		//添加骨骼冲力
		//最后参数：如果为真，则忽略质量，直接作用于运动结果，如果为假，需要计算加速惯性，无法直接作用于速度结果
		GetMesh()->AddImpulse(PointDamageEvent->ShotDirection * Force, PointDamageEvent->HitInfo.BoneName, true);
		//关闭胶囊体碰撞
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		
		OnDead();
		
		DamageValue = 0;
	}
	
	if (OnHPChanged.IsBound())
	{
		OnHPChanged.Broadcast(CurHP, MaxHP);
	}
	
	return DamageValue;
}

