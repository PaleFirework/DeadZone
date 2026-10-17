// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"

#include "DeadZone/Character/DzCharacterBase.h"
#include "DeadZone/Weapon/WeaponActor.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	bRelaxed = (RelaxedTick -= DeltaSeconds) <= 0.f;
	
	//更新角色状态数据
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ADzCharacterBase>(TryGetPawnOwner());
		return;
	}
	
	Velocity = OwnerCharacter->GetVelocity();
	Speed = Velocity.Size2D();//Size2D的长度是抛弃z轴偏移
	bSprint = OwnerCharacter->IsSprinting() && Speed > 0;
	bIsFalling = OwnerCharacter->IsInAir();
	bCrouched = OwnerCharacter->bIsCrouched;
	bShouldMove = Speed > 3.f && !OwnerCharacter->GetCharacterMovement()->GetCurrentAcceleration().IsNearlyZero();
	
	
	bHoldWeapon = OwnerCharacter->GetHoldWeapon() != nullptr;
	
	if (bHoldWeapon || Speed > 0.f || bCrouched)
	{
		bRelaxed = false;
		RelaxedTick = RelaxedTime;
	}
	
	if (bHoldWeapon)
	{
		//计算移动的方向（角度）
		//移动的方向和正方向的角度
		FVector MoveDirection = OwnerCharacter->GetVelocity().GetSafeNormal2D();
		//计算向量夹角余弦值（两条参与运算的向量都是单位向量）
		float CosValue = FVector::DotProduct(MoveDirection, OwnerCharacter->GetActorForwardVector());
		//余弦值转角度，反余弦函数返回来的是弧度
		//RadiansToDegrees将弧度转换位角度
		Direction = FMath::RadiansToDegrees(FMath::Acos(CosValue));
		//还需要进行判定左右，因为左右要通过正负来区分
		if (FVector::DotProduct(MoveDirection, OwnerCharacter->GetActorRightVector()) < 0.f)
		{
			Direction *= -1.f;
		}
		
		bIronsight = OwnerCharacter->IsIronsight();
		AimPitch = OwnerCharacter->GetControlRotation().GetNormalized().Pitch;
		
		HoldWeaponPosition = OwnerCharacter->GetHoldWeapon()->GetHoldSocketPosition();
		//更新同步武器开火
		bFiring = OwnerCharacter->GetHoldWeapon()->GetCurrentState() == EWeaponState::EWS_Firing;
	}
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	RelaxedTick = RelaxedTime;
}

void UPlayerAnimInstance::AnimNotify_RelaxedEnd()
{
	bRelaxed = false;
	RelaxedTick = RelaxedTime;
}
