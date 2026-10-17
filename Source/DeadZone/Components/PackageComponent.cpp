// Fill out your copyright notice in the Description page of Project Settings.


#include "PackageComponent.h"

#include "DeadZone/Character/DzCharacterBase.h"
#include "DeadZone/Core/DzGameDefines.h"
#include "DeadZone/Player/DzPlayerCharacter.h"
#include "DeadZone/Sence/SceneItemActor.h"
#include "DeadZone/Sence/SceneStorageActor.h"
#include "DeadZone/Weapon/WeaponActor.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UPackageComponent::UPackageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	
}

void UPackageComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	ASceneBaseActor* BestFocusItem = GetBestFocusItem();
	if (CurrentBest != BestFocusItem)
	{
		if (IsValid(CurrentBest))
		{
			CurrentBest->SetItemPrompt(false);
		}
		if (IsValid(BestFocusItem))
		{
			BestFocusItem->SetItemPrompt(true);
		}
		
		CurrentBest = BestFocusItem;
	}
	
}

void UPackageComponent::BeginPlay()
{
	Super::BeginPlay();

	ADzPlayerCharacter* Player = Cast<ADzPlayerCharacter>(GetOwner());
	
	if (Player && Player->IsPlayerControlled())
	{
		SetComponentTickEnabled(true);
	}
}


void UPackageComponent::AddNearItemActor(ASceneItemActor* ItemActor)
{
	NearSceneItems.Add(ItemActor);
	
	if (OnAddNearItem.IsBound() && !ItemActor->GetIsDrag())
	{
		OnAddNearItem.Broadcast(ItemActor);
	}
}

void UPackageComponent::RemoveNearItemActor(ASceneItemActor* ItemActor)
{
	NearSceneItems.Remove(ItemActor);
	ItemActor->SetIsDrag(false);
	if (OnRemoveNearItem.IsBound())
	{
		OnRemoveNearItem.Broadcast(ItemActor);
	}
}

void UPackageComponent::AddNearStorageActor(ASceneStorageActor* ItemActor)
{
	NearSceneStorages.Add(ItemActor);
}

void UPackageComponent::RemoveNearStorageActor(ASceneStorageActor* ItemActor)
{
	NearSceneStorages.Remove(ItemActor);
}


ASceneItemActor* UPackageComponent::SpawnSceneItemFromNear(int32 ID, int32 Amount, bool IsDrap)
{
	//从玩家的正前方的180°内随机一个角度
	//RotateAngleAxis让一个向量绕着另外一个向量旋转
	FVector Diretion = GetOwner()->GetActorForwardVector().RotateAngleAxis(FMath::RandRange(-90.f, 90.f), FVector::UpVector);
	
	FVector Pos = GetOwner()->GetActorLocation() + Diretion * FMath::RandRange(50.f, 100.f);
	
	//解决悬空问题 可以从当前位置垂直向下发射射线
	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, Pos, Pos + FVector::UpVector * -10000, ECC_Visibility))
	{
		Pos = Hit.Location;
	}
	
	FTransform Tran;
	Tran.SetLocation(Pos);
	//滞后生成Actor
	//内存中已经创建好Actor但是框架没有处理Actor对象的任何逻辑
	ASceneItemActor* SceneItemActor = GetWorld()->SpawnActorDeferred<ASceneItemActor>(ASceneItemActor::StaticClass(), Tran);
	SceneItemActor->SetID(ID);
	SceneItemActor->SetAmount(Amount);
	SceneItemActor->SetIsDrag(IsDrap);
	SceneItemActor->FinishSpawning(Tran);//这个函数的目的是告诉引擎我已经准备好了，开始走正常执行逻辑流程
	return SceneItemActor;
}

bool UPackageComponent::AddItemAuto(int32 ID, int32 Amount)
{
	const FPropsBase* PropsBase = GetWorld()->GetGameInstance()->GetSubsystem<UPropsSubsystem>()->GetProps(ID);
	if (!PropsBase || !PropsBase->bCanAddToPackage)
	{
		return false;
	}
	
	if (!OnAddItemToPackage.IsBound())
	{
		return false;
	}

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this, ID, Amount]()
	{
		OnAddItemToPackage.Execute(ID, Amount);
	});

	GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);

	return true;
}

bool UPackageComponent::AddItemToStorage(int32 ID, int32 Amount)
{
	if (!OnAddItemToStorage.IsBound())
	{
		return false;
	}

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this, ID, Amount]()
	{
		OnAddItemToStorage.Execute(ID, Amount);
	});

	GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);

	return true;
}

void UPackageComponent::DestroyItemFromNear(ASceneItemActor* SceneItemActor)
{
	if (!IsValid(SceneItemActor))
	{
		return;
	}
	SceneItemActor->Destroy();
}

ASceneItemActor* UPackageComponent::RemoveItemFromPackage(int32 ID, int32 Amount, bool IsDrap)
{
	if (ID >= 0)
	{
		if (!PlayerIsStorage())
		{
			//将道具生成在附近
			return SpawnSceneItemFromNear(ID, Amount, IsDrap);
		} else
		{
			// TODO
			AddItemToStorage(ID, Amount);
		}
	}
	return nullptr;
}

void UPackageComponent::EquipWeapon(int32 ID)
{
	const FPropsBase* PropsBase = GetWorld()->GetGameInstance()->GetSubsystem<UPropsSubsystem>()->GetProps(ID);
	if (!PropsBase || PropsBase->PropsType != EPropsType::EPT_Weapon)
	{
		return;
	}
	const FWeaponHeader* WeaponHeader = static_cast<const FWeaponHeader*>(PropsBase);
	
	if (HoldWeapon)
	{
		//把武器数据放回背包内
		UnEquipWeapon();
	}
	
	//实例武器数据
	HoldWeapon = GetWorld()->SpawnActor<AWeaponActor>(WeaponHeader->WeaponClass);
	if (HoldWeapon)
	{
		HoldWeapon->SetID(ID);
		HoldWeapon->SetMaster(Cast<ADzCharacterBase>(GetOwner()));
		
		if (OnEquipWeapon.IsBound())
		{
			OnEquipWeapon.Broadcast(HoldWeapon->GetID());
		}
	}
	
}

void UPackageComponent::UnEquipWeapon()
{
	AddItemAuto(HoldWeapon->GetID());
	HoldWeapon->Destroy();
	HoldWeapon = nullptr;
	if (OnUnEquipWeapon.IsBound())
	{
		OnUnEquipWeapon.Broadcast(-1);
	}
}

bool UPackageComponent::PutOnEquipment(int32 ID)
{
	const FPropsBase* PropsBase = GetWorld()->GetGameInstance()->GetSubsystem<UPropsSubsystem>()->GetProps(ID);
	if (!PropsBase)
	{
		return false;
	}
	if (PropsBase->PropsType == EPropsType::EPT_Weapon)
	{
		EquipWeapon(ID);
	} else
	{
		const FEquipmentHeader* EquipmentHeader = static_cast<const FEquipmentHeader*>(PropsBase);
		EEquipmentType EquipmentType = EquipmentHeader->EquipmentType;
	
		if (OnPutOnEquipment.IsBound())
		{
			OnPutOnEquipment.Broadcast(EquipmentType, ID);
		}
		
		if (EquipmentMap.Contains(EquipmentType))
		{
			AddItemAuto(EquipmentMap[EquipmentType]);
			EquipmentMap[EquipmentType] = ID;
		}
		else
		{
			EquipmentMap.Add(EquipmentType, ID);
		}
	}
	
	
	return true;
}

int32 UPackageComponent::TakeOffEquipment(EEquipmentType EquipmentType)
{
	if (EquipmentMap.Contains(EquipmentType))
	{
		int32 ID = EquipmentMap[EquipmentType];
		
		EquipmentMap.Remove(EquipmentType);
		if (OnTakeoffEquipment.IsBound())
		{
			OnTakeoffEquipment.Broadcast(EquipmentType, ID);
		}
		
		return ID;
	}
	return -1;
}

void UPackageComponent::ShowPackageInventory()
{
	if (OnShowPackage.IsBound())
	{
		OnShowPackage.Broadcast();
	}
}

void UPackageComponent::UnShowPackageInventory()
{
	if (OnUnShowPackage.IsBound())
	{
		OnUnShowPackage.Broadcast();
	}
}

void UPackageComponent::Interact()
{
	if (TObjectPtr<ASceneItemActor> CurrentBestItem = Cast<ASceneItemActor>(CurrentBest))
	{
		if (CurrentBestItem->GetID() < CONSUMABLE_INDEX)
		{
			if (PutOnEquipment(CurrentBestItem->GetID()))
			{
				DestroyItemFromNear(CurrentBestItem);
			}
		} else
		{
			if (AddItemAuto(CurrentBestItem->GetID(), CurrentBestItem->GetAmount()))
			{
				DestroyItemFromNear(CurrentBestItem);
			}
		}
		
		return;
	}
	if (TObjectPtr<ASceneStorageActor> CurrentBestStorage = Cast<ASceneStorageActor>(CurrentBest))
	{
		ADzPlayerCharacter* Player = Cast<ADzPlayerCharacter>(GetOwner());
		Player->Storage();
		CurrentBestStorage->ChangePrompt(Player->GetIsStorage());
		if (Player->GetIsStorage())
		{
			OnInventoryChanged.Execute(CurrentBestStorage->GetItems());
		} else
		{
			OnInventorySaved.Execute(CurrentBestStorage);
		}
	}
}

bool UPackageComponent::PlayerIsStorage()
{
	return Cast<ADzPlayerCharacter>(GetOwner())->GetIsStorage();
}

int32 UPackageComponent::ConsumeAmmo(int32 BulletID ,int32 NeededAmount)
{
	return OnConsumeAmmo.Execute(BulletID, NeededAmount);
}

ASceneBaseActor* UPackageComponent::GetBestFocusItem()
{
	// 1. 获取玩家摄像机的位置和朝向
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (!CameraManager) return nullptr;

	FVector CameraLocation = CameraManager->GetCameraLocation();
	FVector CameraForward = CameraManager->GetActorForwardVector().GetSafeNormal();

	ASceneBaseActor* BestItem = nullptr;
	float MaxDot = -1.0f;

	// 设置夹角阈值（例如 0.866f 对应约 30 度扇形区域，小于此角度才可被视线选中）
	const float DotThreshold = 0.85f; 
	
	for (int32 i = NearSceneItems.Num() - 1; i >= 0; --i)
	{
		ASceneItemActor* Item = NearSceneItems[i].Get();
		
		float DistanceToCharacter = FVector::Distance(GetOwner()->GetActorLocation(), Item->GetActorLocation());
		if (DistanceToCharacter > 200) continue;

		// 计算从相机指向物体的单位向量
		FVector DirToItem = (Item->GetActorLocation() - CameraLocation).GetSafeNormal();
		float DotValue = FVector::DotProduct(CameraForward, DirToItem);

		// 筛选点积最大（最靠近视线中央）且符合门槛的物品
		if (DotValue > DotThreshold && DotValue > MaxDot)
		{
			MaxDot = DotValue;
			BestItem = Item;
		}
	}
	for (int32 i = NearSceneStorages.Num() - 1; i >= 0; --i)
	{
		ASceneStorageActor* Item = NearSceneStorages[i].Get();
		
		float DistanceToCharacter = FVector::Distance(GetOwner()->GetActorLocation(), Item->GetActorLocation());
		if (DistanceToCharacter > 200) continue;

		// 计算从相机指向物体的单位向量
		FVector DirToItem = (Item->GetActorLocation() - CameraLocation).GetSafeNormal();
		float DotValue = FVector::DotProduct(CameraForward, DirToItem);

		// 筛选点积最大（最靠近视线中央）且符合门槛的物品
		if (DotValue > DotThreshold && DotValue > MaxDot)
		{
			MaxDot = DotValue;
			BestItem = Item;
		}
	}

	return BestItem;
}



