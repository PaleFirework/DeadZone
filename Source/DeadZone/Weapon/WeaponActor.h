// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"


class ADzCharacterBase;

UENUM()
enum class EWeaponState : uint8
{
	EWS_Normal,
	EWS_Reloading,
	EWS_Firing,
	EWS_Empty,
};

DECLARE_MULTICAST_DELEGATE_TwoParams(DelegateWeaponClipChanged, int32, int32);

UCLASS()
class DEADZONE_API AWeaponActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void SpawnBullet();
	
	void SpawnEffect();
	
	void SpawnDamage();
	
	bool GetFirePositionAndDirection(FVector& Position, FVector& Direction);
	
	//NET
	UFUNCTION(Server, Unreliable)
	void Server_StartFire();
	UFUNCTION(Server, Unreliable)
	void Server_StopFire();
	
	UFUNCTION()
	void OnRep_CurrentClip();
	UFUNCTION(Server, Unreliable)
	void Server_ReloadWeapon();
	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayReloadMontage();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void SetID(int32 InID) { ID = InID; }
	int32 GetID() const { return ID; }
	int32 GetCurrentClip() const { return CurrentClip; }
	int32 GetMaxClipVolume() const { return MaxClipVolume; }
	EWeaponState GetCurrentState() const { return CurrentState; }
	
	void SetMaster(TObjectPtr<ADzCharacterBase> InMaster);
	FVector GetHoldSocketPosition();
	
	void StartFire();
	void StopFire();
	void ReloadWeapon();
	void MakeFullClip();
	
protected:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	UPROPERTY(EditAnywhere)
	int32 MaxClipVolume = 30;
	UPROPERTY(EditAnywhere)
	int32 BulletID;
	UPROPERTY(EditAnywhere)
	float FireInterval = 0.1f;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundCue> FireSound;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundCue> DryFireSound;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> FireEffect;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ShellCasingClass;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> HitEffect;
	UPROPERTY(EditAnywhere)
	FName HoldSocketName = TEXT("WeaponSocket");
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> ReloadMontage;
	UPROPERTY(EditAnywhere)
	float ShotDistance = 50000;
	UPROPERTY(Replicated)
	int32 ID;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentClip)
	int32 CurrentClip;
	UPROPERTY(Replicated)
	TObjectPtr<ADzCharacterBase> Master;
	
	FTimerHandle FireTimerHandle;
	
	float LastFireTime;
	UPROPERTY(Replicated)
	EWeaponState CurrentState;
	
public:
	
	DelegateWeaponClipChanged OnWeaponClipChanged;
};
