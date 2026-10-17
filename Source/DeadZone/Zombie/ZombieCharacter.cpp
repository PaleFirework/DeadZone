// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieCharacter.h"

#include "VectorTypes.h"
#include "ZombieController.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "DeadZone/Character/DzCharacterBase.h"
#include "DeadZone/GamePlay/Game/DzGameMode.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


// Sets default values
AZombieCharacter::AZombieCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AIControllerClass = AZombieController::StaticClass();
	//修改AI控制器的启动方式
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
	
	CurHP = MaxHP;
}

// Called when the game starts or when spawned
void AZombieCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayRandomSound();
}

float AZombieCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if (IsDead())
	{
		return 0;
	}
	int32 DamageValue = 0;
	
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
		
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);
		GetMesh()->SetSimulatePhysics(true);
		
		//添加骨骼冲力
		//最后参数：如果为真，则忽略质量，直接作用于运动结果，如果为假，需要计算加速惯性，无法直接作用于速度结果
		GetMesh()->AddImpulse(PointDamageEvent->ShotDirection * Force, PointDamageEvent->HitInfo.BoneName, true);
		//关闭胶囊体碰撞
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		
		OnDead();
		
		DamageValue = 0;
	}
	
	
	return DamageValue;
}

void AZombieCharacter::PlayRandomSound()
{
	if (IsDead())
	{
		GetWorldTimerManager().ClearTimer(SoundTimerHandle);
		return;
	}

	if (ZombieAudioCues.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, ZombieAudioCues.Num() - 1);
		USoundCue* SelectedCue = ZombieAudioCues[RandomIndex];

		if (SelectedCue)
		{
			CurrentZombieAudioComponent =
				UGameplayStatics::SpawnSoundAtLocation(
					this,
					SelectedCue,
					GetActorLocation()
				);
		}
	}

	float RandomDelay = FMath::RandRange(3.0f, 5.0f);

	GetWorldTimerManager().SetTimer(
		SoundTimerHandle,
		this,
		&AZombieCharacter::PlayRandomSound,
		RandomDelay,
		false
	);
}

void AZombieCharacter::OnDead()
{
	GetWorldTimerManager().ClearTimer(SoundTimerHandle);

	// 当前正在播放的叫声淡出
	if (CurrentZombieAudioComponent)
	{
		CurrentZombieAudioComponent->FadeOut(
			1.5f,  // 1秒淡出
			0.0f  // 最终音量
		);
	}
}

void AZombieCharacter::RotateToTarget(AActor* TargetActor, float DeltaSeconds)
{
	if (!TargetActor)
	{
		return;
	}

	const FVector Direction = TargetActor->GetActorLocation() - GetActorLocation();

	if (Direction.IsNearlyZero())
	{
		return;
	}

	FRotator TargetRotation = Direction.Rotation();
	TargetRotation.Pitch = 0.f;
	TargetRotation.Roll = 0.f;

	const FRotator NewRotation = FMath::RInterpConstantTo(
		GetActorRotation(),
		TargetRotation,
		DeltaSeconds,
		360.f
	);

	SetActorRotation(NewRotation);
}

UAnimMontage* AZombieCharacter::PlayTurnMontage(float Angle)
{
	// Angle：
	// 正数 = 右转
	// 负数 = 左转
	const float AbsAngle = FMath::Abs(Angle);

	UAnimMontage* Montage = nullptr;

	if (Angle > 0.f)
	{
		// 右转
		if (AbsAngle <= 90.f)
		{
			Montage = TurnRight90Montage;
		}
		else
		{
			Montage = TurnRight180Montage;
		}
	}
	else
	{
		// 左转
		if (AbsAngle <= 90.f)
		{
			Montage = TurnLeft90Montage;
		}
		else
		{
			Montage = TurnLeft180Montage;
		}
	}

	if (Montage)
	{
		PlayAnimMontage(Montage, FMath::FRandRange(2.0f, 2.5f));
	}

	return Montage;
}

// Called every frame
void AZombieCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AZombieCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool AZombieCharacter::IsDead() const
{
	return CurHP <= 0;
}


