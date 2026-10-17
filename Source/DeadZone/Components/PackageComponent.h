// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DeadZone/Subsystem/PropsSubsystem.h"
#include "PackageComponent.generated.h"


class ASceneStorageActor;
class ASceneBaseActor;
enum class EEquipmentType : uint8;
class AWeaponActor;
class ASceneItemActor;

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count = 0;
};
using FInventoryMap = TMap<int32, FInventorySlot>;

DECLARE_MULTICAST_DELEGATE(DelegateShowPackage);
DECLARE_MULTICAST_DELEGATE_OneParam(DelegateNearItem, ASceneItemActor*);
DECLARE_MULTICAST_DELEGATE_TwoParams(DelegateEquipmentChanged, EEquipmentType, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(DelegateWeaponChanged, int32);

DECLARE_DELEGATE_RetVal_TwoParams(bool, FOnAddItemToPackage, int32, int32);
DECLARE_DELEGATE_RetVal_TwoParams(bool, FOnAddItemToStorage, int32, int32);
DECLARE_DELEGATE_OneParam(FOnInventoryChanged, FInventoryMap);
DECLARE_DELEGATE_OneParam(FOnInventorySaved, ASceneStorageActor*);
DECLARE_DELEGATE_RetVal_TwoParams(int32 ,FOnConsumeAmmo, int32, int32);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEADZONE_API UPackageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPackageComponent();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
	
	TArray<TObjectPtr<ASceneItemActor>> GetNearItemActors() const { return NearSceneItems; }
	TObjectPtr<AWeaponActor> GetHoldWeapon() const { return HoldWeapon; }
	
	void AddNearItemActor(ASceneItemActor* ItemActor);
	void RemoveNearItemActor(ASceneItemActor* ItemActor);
	void AddNearStorageActor(ASceneStorageActor* ItemActor);
	void RemoveNearStorageActor(ASceneStorageActor* ItemActor);
	
	ASceneItemActor* SpawnSceneItemFromNear(int32 ID,int32 Amount, bool IsDrap);
	
	bool AddItemAuto(int32 ID, int32 Amount=1);
	bool AddItemToStorage(int32 ID, int32 Amount=1);
	void DestroyItemFromNear(ASceneItemActor* SceneItemActor);
	ASceneItemActor* RemoveItemFromPackage(int32 ID, int32 Amount, bool IsDrap=true);
	
	void EquipWeapon(int32 ID);
	void UnEquipWeapon();
	
	bool PutOnEquipment(int32 ID);
	int32 TakeOffEquipment(EEquipmentType EquipmentType);
	
	void ShowPackageInventory();
	void UnShowPackageInventory();
	void Interact();
	bool PlayerIsStorage();
	
	int32 ConsumeAmmo(int32 BulletID ,int32 NeededAmount);

protected:
	ASceneBaseActor* GetBestFocusItem();


public:
	DelegateShowPackage OnShowPackage;
	DelegateShowPackage OnUnShowPackage;
	
	DelegateNearItem OnAddNearItem;
	DelegateNearItem OnRemoveNearItem;

	DelegateEquipmentChanged OnPutOnEquipment;
	DelegateEquipmentChanged OnTakeoffEquipment;
	
	DelegateWeaponChanged OnEquipWeapon;
	DelegateWeaponChanged OnUnEquipWeapon;
	
	FOnAddItemToPackage OnAddItemToPackage;
	FOnAddItemToStorage OnAddItemToStorage;
	
	FOnInventoryChanged OnInventoryChanged;
	FOnInventorySaved OnInventorySaved;
	
	FOnConsumeAmmo OnConsumeAmmo;
	
protected:
	UPROPERTY()
	TObjectPtr<ASceneBaseActor> CurrentBest;
	UPROPERTY()
	TArray<TObjectPtr<ASceneItemActor>> NearSceneItems;
	UPROPERTY()
	TArray<TObjectPtr<ASceneStorageActor>> NearSceneStorages;
	UPROPERTY()
	TObjectPtr<AWeaponActor> HoldWeapon;
	
	TMap<int32, int32> PackageMap;
	TMap<EEquipmentType, int32> EquipmentMap;
	
};
