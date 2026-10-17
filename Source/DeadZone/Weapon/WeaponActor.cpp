// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"

#include "Camera/CameraComponent.h"
#include "DeadZone/Character/DzCharacterBase.h"
#include "DeadZone/Components/PackageComponent.h"
#include "DeadZone/Core/DzGameDefines.h"
#include "DeadZone/Player/DzPlayerCharacter.h"
#include "DeadZone/Zombie/ZombieCharacter.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PrimaryActorTick.bCanEverTick = true;
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	SetRootComponent(SkeletalMeshComponent);
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	CurrentClip = MaxClipVolume;
}

void AWeaponActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AWeaponActor, ID, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(AWeaponActor, Master, COND_InitialOnly);
	DOREPLIFETIME(AWeaponActor, CurrentState);
	DOREPLIFETIME(AWeaponActor, CurrentClip);
}

void AWeaponActor::SpawnBullet()
{
	if (CurrentClip <= 0)
	{
		return;
	}
	
	SpawnDamage();
	if (--CurrentClip > 0)
	{
		//启动定时器，继续发射子弹
		
		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ThisClass::SpawnBullet, FireInterval);
	}
	//裁定是否是开火状态
	CurrentState = CurrentClip > 0 ? EWeaponState::EWS_Firing : EWeaponState::EWS_Empty;
	//记录发射时间
	LastFireTime = GetWorld()->GetTimeSeconds();//GetTimeSeconds获取当前游戏运行的时间
	
	OnRep_CurrentClip();
}

void AWeaponActor::SpawnEffect()
{
	if (FireEffect)
	{
		//生成粒子
		UGameplayStatics::SpawnEmitterAttached(FireEffect, SkeletalMeshComponent, TEXT("Muzzle"));
	}
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, SkeletalMeshComponent->GetSocketLocation(TEXT("Muzzle")), SkeletalMeshComponent->GetSocketRotation(TEXT("Muzzle")));
	}
	if (ShellCasingClass)
	{
		FVector EjectLocation = SkeletalMeshComponent->GetSocketLocation(TEXT("EjectionPort"));
		FRotator EjectRotation = SkeletalMeshComponent->GetSocketRotation(TEXT("EjectionPort"));

		// 2. 配置生成参数
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		
		AActor* SpawnedShell = GetWorld()->SpawnActor<AActor>(ShellCasingClass, EjectLocation, EjectRotation, SpawnParams);

		
		if (SpawnedShell)
		{
			UPrimitiveComponent* ShellMesh = Cast<UPrimitiveComponent>(SpawnedShell->GetRootComponent());
			if (ShellMesh && ShellMesh->IsSimulatingPhysics())
			{
				// 沿着插槽右侧（或自定义方向）飞出，加点随机扰动更自然
				FVector EjectDirection = EjectRotation.RotateVector(FVector(0.0f, 1.0f, 0.5f)); // 向右上方抛出
             
				// 增加微小随机偏转，避免每次抛壳轨迹完全重合
				EjectDirection += FMath::VRand() * 0.1f; 

				float EjectForce = FMath::RandRange(500.0f, 700.0f); // 抛出力度
				ShellMesh->AddImpulse(EjectDirection * EjectForce, NAME_None, true);
			}
		}
	}
}

void AWeaponActor::SpawnDamage()
{
	FVector Position;
	FVector Direction;
	bool IsHit = GetFirePositionAndDirection(Position, Direction);
	
	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, Position, Direction * ShotDistance + Position, WEAPON_TRACE))
	{
		AActor* HitActor = Hit.GetActor();
		if (!Cast<ADzCharacterBase>(HitActor) && !Cast<AZombieCharacter>(HitActor) && IsHit)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				HitEffect,
				Hit.ImpactPoint,
				Hit.ImpactNormal.Rotation()
			);
			return;
		}
		
		//构建一个点伤害的Event
		FPointDamageEvent PointDamageEvent;
		PointDamageEvent.HitInfo = Hit;
		PointDamageEvent.ShotDirection = Direction;
		
		//传递伤害
		float Value = Hit.GetActor()->TakeDamage(1, PointDamageEvent, Master->GetController(), this);
	}
	
	//DrawDebugLine(GetWorld(), Position, Position + Direction * ShotDistance, FColor::Red, false, 2);
}

bool AWeaponActor::GetFirePositionAndDirection(FVector& Position, FVector& Direction)
{
	Position = SkeletalMeshComponent->GetSocketLocation(TEXT("Muzzle"));
	Direction = SkeletalMeshComponent->GetSocketQuaternion(TEXT("Muzzle")).GetAxisX();
	//玩家的射击方向和AI不一样
	if (APlayerController* Pc = Cast<APlayerController>(Master->GetController()))
	{
		if (ADzPlayerCharacter* Player = Cast<ADzPlayerCharacter>(Master))
		{
			FVector WorldLocation = Player->GetCameraComponent()->GetComponentLocation();
			FVector WorldDirection = Player->GetCameraComponent()->GetComponentRotation().Vector();
			//发射检测射线
			FHitResult Hit;
			
			if (GetWorld()->LineTraceSingleByChannel(Hit, WorldLocation, WorldLocation + WorldDirection * ShotDistance, WEAPON_TRACE))
			{
				FVector _Direct = (Hit.Location - Position).GetSafeNormal();//屏幕中心击中的位置，然后从枪口朝向击中位置的向量
				if (FVector::DotProduct(Direction, _Direct) > 0)
				{
					Direction = _Direct;
				}
				return true;
			}
		}
		
	}
	else if (Master)
	{
		//AI的射击相关内容逻辑
		FHitResult Hit;
		
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Master);
		
		FVector FireAliPos = Master->GetActorLocation() + FVector::UpVector * FMath::FRandRange(-80.f, 90.f) + Master->GetActorRightVector() * FMath::FRandRange(-20.f, 20.f) ;
		
		if (GetWorld()->LineTraceSingleByChannel(Hit, FireAliPos, FireAliPos + Master->GetControlRotation().Vector() * ShotDistance, WEAPON_TRACE, QueryParams))
		{
			FVector _Direct = (Hit.Location - Position).GetSafeNormal();//击中的位置，然后从枪口朝向击中位置的向量
			if (FVector::DotProduct(Direction, _Direct) > 0)
			{
				Direction = _Direct;
			}
			return true;
		}
		
	}
	return false;
}

void AWeaponActor::Server_StartFire_Implementation()
{
	StartFire();
}

void AWeaponActor::Server_StopFire_Implementation()
{
	StopFire();
}

void AWeaponActor::OnRep_CurrentClip()
{
	if (CurrentClip != MaxClipVolume)
	{
		SpawnEffect();
	}
	if (OnWeaponClipChanged.IsBound())
	{
		OnWeaponClipChanged.Broadcast(CurrentClip, MaxClipVolume);
	}
}

void AWeaponActor::Server_ReloadWeapon_Implementation()
{
	ReloadWeapon();
}

void AWeaponActor::Multi_PlayReloadMontage_Implementation()
{
	if (ReloadMontage && Master)
	{
		Master->PlayAnimMontage(ReloadMontage, 1, Master->IsIronsight() ? TEXT("Ironsight") : TEXT("Default"));
	}
}

// Called every frame
void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponActor::SetMaster(TObjectPtr<ADzCharacterBase> InMaster)
{
	Master = InMaster;
	//让枪械依附到角色的身上
	if (Master)
	{
		SkeletalMeshComponent->AttachToComponent(Master->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, HoldSocketName);
		SetOwner(InMaster);//必须要设置Owner，否则客户端无法在自己的终端给无服务器发消息
	}
}

FVector AWeaponActor::GetHoldSocketPosition()
{
	return SkeletalMeshComponent->GetSocketLocation(TEXT("HoldSocket"));
}

void AWeaponActor::StartFire()
{
	if (!HasAuthority())
	{
		Server_StartFire();
		return;
	}
	if (CurrentState == EWeaponState::EWS_Reloading || CurrentState == EWeaponState::EWS_Firing)
	{
		return;
	}
	if (CurrentState == EWeaponState::EWS_Empty)
	{
		if (DryFireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DryFireSound, SkeletalMeshComponent->GetSocketLocation(TEXT("Muzzle")), SkeletalMeshComponent->GetSocketRotation(TEXT("Muzzle")));
		}
	}
	if (GetWorld()->GetTimeSeconds() -  LastFireTime < FireInterval)
	{
		return;
	}
	SpawnBullet();
}

void AWeaponActor::StopFire()
{
	if (!HasAuthority())
	{
		Server_StopFire();
		return;
	}
	//检查发射的定时器是否有工作
	if (FireTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
	}
	if (CurrentState == EWeaponState::EWS_Firing)
	{
		CurrentState = EWeaponState::EWS_Normal;
	}
}

void AWeaponActor::ReloadWeapon()
{
	if (CurrentState == EWeaponState::EWS_Reloading)
	{
		return;
	}
	if (!HasAuthority())
	{
		Server_ReloadWeapon();
		return;
	}
	
	if (ADzPlayerCharacter* Player = Cast<ADzPlayerCharacter>(Master))
	{
		Player->GetPackageComponent()->ConsumeAmmo(BulletID, MaxClipVolume-CurrentClip);
	}
	
	StopFire();
	CurrentState = EWeaponState::EWS_Reloading;
	
	/*if (ReloadMontage && Master)
	{
		Master->PlayAnimMontage(ReloadMontage, 1, Master->IsIronsight() ? TEXT("Ironsight") : TEXT("Default"));
	}*/
	Multi_PlayReloadMontage();
}


void AWeaponActor::MakeFullClip()
{
	CurrentClip = MaxClipVolume;
	CurrentState = EWeaponState::EWS_Normal;
	OnRep_CurrentClip();
}

