// Fill out your copyright notice in the Description page of Project Settings.


#include "PropsSubsystem.h"

#include "DeadZone/Core/DzGameDefines.h"

void UPropsSubsystem::LoadDataTable()
{
	if (!EquipmentDataTable)
	{
		EquipmentDataTable = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/DeadZone/Data/Props/DT_Equipment.DT_Equipment'"));
		//解析数据到内存
		for (auto KV : EquipmentDataTable->GetRowMap())
		{
			int32 ID = -1;
			FString Left, Right;
			if (KV.Key.ToString().Split(TEXT("_"), &Left, &Right))//查找有没有分隔符，如果有返回真，没有返回假
			{
				//如何将FString转换为数字
				ID = FCString::Atoi(*Right);
			}
			//往map中装填数据了
			if (!PropsData.Contains(ID))
			{
				PropsData.Add(ID, reinterpret_cast<FEquipmentHeader*>(KV.Value));
			}
		}
	}
	
	if (!WeaponDataTable)
	{
		WeaponDataTable = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/DeadZone/Data/Props/DT_Weapon.DT_Weapon'"));
		//解析数据到内存
		for (auto KV : WeaponDataTable->GetRowMap())
		{
			int32 ID = WEAPON_INDEX;
			FString Left, Right;
			if (KV.Key.ToString().Split(TEXT("_"), &Left, &Right))
			{
				//如何将FString转换为数字
				ID = FCString::Atoi(*Right) + WEAPON_INDEX;
			}

			if (!PropsData.Contains(ID))
			{
				PropsData.Add(ID, reinterpret_cast<FPropsBase*>(KV.Value));
			}
		}
	}
	
	if (!ConsumableDataTable)
	{
		ConsumableDataTable = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/DeadZone/Data/Props/DT_Consumable.DT_Consumable'"));
		//解析数据到内存
		for (auto KV : ConsumableDataTable->GetRowMap())
		{
			int32 ID = CONSUMABLE_INDEX;
			FString Left, Right;
			if (KV.Key.ToString().Split(TEXT("_"), &Left, &Right))
			{
				//如何将FString转换为数字
				ID = FCString::Atoi(*Right) + CONSUMABLE_INDEX;
			}

			if (!PropsData.Contains(ID))
			{
				PropsData.Add(ID, reinterpret_cast<FPropsBase*>(KV.Value));
			}
		}
	}
	
	if (!StorageDataTable)
	{
		StorageDataTable = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/DeadZone/Data/Props/DT_Storage.DT_Storage'"));
		//解析数据到内存
		for (auto KV : StorageDataTable->GetRowMap())
		{
			int32 ID = -1;
			FString Left, Right;
			if (KV.Key.ToString().Split(TEXT("_"), &Left, &Right))
			{
				//如何将FString转换为数字
				ID = FCString::Atoi(*Right);
			}

			if (!StorageData.Contains(ID))
			{
				StorageData.Add(ID, reinterpret_cast<FStorageBase*>(KV.Value));
			}
		}
	}
}

const FPropsBase* UPropsSubsystem::GetProps(int32 ID)
{
	LoadDataTable();
	if (PropsData.Contains(ID))
	{
		return PropsData[ID];
	}
	return nullptr;
}

const FStorageBase* UPropsSubsystem::GetStorage(int32 ID)
{
	LoadDataTable();
	if (StorageData.Contains(ID))
	{
		return StorageData[ID];
	}
	return nullptr;
}
