// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PropsSubsystem.generated.h"


class AWeaponActor;

UENUM()
enum class EPropsType : uint8
{
	EPT_Equipment,
	EPT_Weapon,
	EPT_Consumable,
};

UENUM()
enum class EEquipmentType : uint8
{
	EST_None,
	EST_Hair,
	EST_Cap,
	EST_Helmet,
	EST_Clothing,
	EST_Glasses,
	EST_Masker,
	EST_Beard,
	EST_Package,
};

USTRUCT()
struct FStorageBase : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FText Name;
	UPROPERTY(EditAnywhere)
	int32 StorageMax;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> StaticMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh> SkeletalMesh;
};

USTRUCT()
struct FPropsBase : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FText Name;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Icon;
	UPROPERTY(EditAnywhere)
	bool bCanAddToPackage = true;
	EPropsType PropsType;
};

USTRUCT()
struct FEquipmentHeader : public FPropsBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	EEquipmentType EquipmentType;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> StaticMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh> SkeletalMesh;
};

USTRUCT()
struct FWeaponHeader : public FPropsBase
{
	GENERATED_BODY()
	
	FWeaponHeader()
	{
		PropsType = EPropsType::EPT_Weapon;
	}
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh> SkeletalMesh;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeaponActor> WeaponClass;
};

USTRUCT()
struct FConsumableHeader : public FPropsBase
{
	GENERATED_BODY()
	
	FConsumableHeader()
	{
		PropsType = EPropsType::EPT_Consumable;
	}
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> StaticMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh> SkeletalMesh;
	UPROPERTY(EditAnywhere)
	int32 MaxStackSize = 99;
};
/**
 * 
 */
UCLASS()
class DEADZONE_API UPropsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:
	
	void LoadDataTable();
	
	
public:
	const FPropsBase* GetProps(int32 ID);
	const FStorageBase* GetStorage(int32 ID);
	
protected:
	
	UPROPERTY()
	TObjectPtr<UDataTable> EquipmentDataTable;
	UPROPERTY()
	TObjectPtr<UDataTable> WeaponDataTable;
	UPROPERTY()
	TObjectPtr<UDataTable> ConsumableDataTable;
	UPROPERTY()
	TObjectPtr<UDataTable> StorageDataTable;
	
	TMap<int32, FPropsBase*> PropsData;
	TMap<int32, FStorageBase*> StorageData;
};
