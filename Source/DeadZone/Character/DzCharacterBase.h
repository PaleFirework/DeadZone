// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "DeadZone/Interface/DeadInterface.h"
#include "DeadZone/Interface/EquipmentInterface.h"
#include "GameFramework/Character.h"
#include "DzCharacterBase.generated.h"

enum class ETeamType : uint8;
class AWeaponActor;
class UEquipmentComponent;
class UPackageComponent;

DECLARE_MULTICAST_DELEGATE_TwoParams(DelegateHPChanged, int32, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(DelegateAttacked, AController*);

UCLASS()
class DEADZONE_API ADzCharacterBase : public ACharacter, public IAbilitySystemInterface, public IEquipmentInterface, public IDeadInterface
{
	GENERATED_BODY()

public:
	ADzCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AbilitySystem")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	
	TObjectPtr<UPackageComponent> GetPackageComponent() const {
		return PackageComponent;
	}
	TObjectPtr<AWeaponActor> GetHoldWeapon() const;

	bool IsSprinting() const;
	bool IsInAir() const;
	bool IsIronsight() const;
	virtual bool IsDead() const override;
	bool IsCrouch() const;
	
	int32 GetHP() const { return CurHP; }
	int32 GetMaxHP() const { return MaxHP; }
	
	virtual void Jump() override;
	void DoCrouch();
	
	void StartSprint();
	void StopSprint();
	
	void StartFire();
	void StopFire();
	
	void StartIronsight();
	void StopIronsight();
	
	void ReloadWeapon();
	
	ETeamType GetTeamType() const;	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
	EGameplayEffectReplicationMode ReplicationMode = EGameplayEffectReplicationMode::Minimal;
	
	bool bCrouch;
	bool bSprint;
	bool bIronsight;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPackageComponent> PackageComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UEquipmentComponent> EquipmentComponent;
	
	
	UPROPERTY(EditAnywhere)
	ETeamType TeamType;
	
	int32 CurHP;
	UPROPERTY(EditAnywhere)
	int32 MaxHP = 100;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> HurtEffect;

	
protected:
	void OnEquipWeapon(int32 ID);
	void OnUnEquipWeapon(int32 ID);
	
	UFUNCTION(BlueprintCallable)
	void EquipWeapon(int32 ID);
	
	virtual void OnDead();
	
	virtual USkeletalMeshComponent* GetSkeletalMeshComponent() override;

	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
public:
	DelegateHPChanged OnHPChanged;
	DelegateAttacked OnAttacked;
};
