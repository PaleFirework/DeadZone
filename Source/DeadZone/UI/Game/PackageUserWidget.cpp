// Fill out your copyright notice in the Description page of Project Settings.


#include "PackageUserWidget.h"

#include <ThirdParty/hlslcc/hlslcc/src/hlslcc_lib/ir.h>

#include "EquipmentWidget.h"
#include "InventoryWidget.h"
#include "ItemFrameWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "DeadZone/Components/PackageComponent.h"
#include "DeadZone/Player/DzPlayerCharacter.h"
#include "DeadZone/Sence/PlayerModelActor.h"


void UPackageUserWidget::RemoveFromParent()
{
	
	if (ADzPlayerCharacter* Player = Cast<ADzPlayerCharacter>(GetOwningPlayerPawn()))
	{
		Player->GetPackageComponent()->OnAddNearItem.RemoveAll(this);
		Player->GetPackageComponent()->OnRemoveNearItem.RemoveAll(this);
		
		
		Player->GetPackageComponent()->OnPutOnEquipment.RemoveAll(this);
		Player->GetPackageComponent()->OnTakeoffEquipment.RemoveAll(this);
		
		Player->GetPackageComponent()->OnEquipWeapon.RemoveAll(this);
		Player->GetPackageComponent()->OnUnEquipWeapon.RemoveAll(this);
	}
	
	if (PlayerModelActor)
	{
		PlayerModelActor->SetActorHiddenInGame(true);
	}
	Super::RemoveFromParent();
}

void UPackageUserWidget::ShowPackageInventory()
{
	PackageInventory->SetVisibility(ESlateVisibility::Visible);
}

void UPackageUserWidget::UnShowPackageInventory()
{
	PackageInventory->SetVisibility(ESlateVisibility::Hidden);
}

void UPackageUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	NearInventory->SetText(NSLOCTEXT("Package", "Text", "附近"));
	NearInventory->SetNear(true);
	PocketInventory->SetText(NSLOCTEXT("Package", "Text", "口袋"));;
	PackageInventory->SetText(NSLOCTEXT("Package", "Text", "背包"));
	TargetInventory->SetStorage(true);
	TargetInventory->AddItemFrames();
	TargetInventory->AddItemFrames();
	TargetInventory->AddItemFrames();
	PackageInventory->AddItemFrames();
	PackageInventory->AddItemFrames();
	PackageInventory->AddItemFrames();
	PackageInventory->SetVisibility(ESlateVisibility::Hidden);
	
	if (ADzPlayerCharacter* Player = Cast<ADzPlayerCharacter>(GetOwningPlayerPawn()))
	{
		Player->GetPackageComponent()->OnShowPackage.AddUObject(this, &ThisClass::ShowPackageInventory);
		Player->GetPackageComponent()->OnUnShowPackage.AddUObject(this, &ThisClass::UnShowPackageInventory);
		
		Player->GetPackageComponent()->OnAddItemToPackage.BindUObject(this, &ThisClass::OnAddItemToPackage);
		Player->GetPackageComponent()->OnAddItemToStorage.BindUObject(this, &ThisClass::OnAddItemToStorage);
		Player->GetPackageComponent()->OnInventoryChanged.BindUObject(this, &ThisClass::OnInventoryChanged);
		Player->GetPackageComponent()->OnInventorySaved.BindUObject(this, &ThisClass::OnInventorySaved);
		Player->GetPackageComponent()->OnConsumeAmmo.BindUObject(this, &ThisClass::OnConsumeAmmo);
	}
}

void UPackageUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	if (ADzPlayerCharacter* Player = Cast<ADzPlayerCharacter>(GetOwningPlayerPawn()))
	{
		for (auto Item  : Player->GetPackageComponent()->GetNearItemActors())
		{
			NearInventory->AddNearItemActor(Item);
		}
		//绑定代理
		Player->GetPackageComponent()->OnAddNearItem.AddUObject(this, &ThisClass::OnAddNearItemActor);
		Player->GetPackageComponent()->OnRemoveNearItem.AddUObject(this, &ThisClass::OnRemoveNearItemActor);
		
		//绑定穿脱装饰性部件
		Player->GetPackageComponent()->OnPutOnEquipment.AddUObject(this, &ThisClass::OnPutOnEquipment);
		Player->GetPackageComponent()->OnTakeoffEquipment.AddUObject(this, &ThisClass::OnTakeoffEquipment);
		
		Player->GetPackageComponent()->OnEquipWeapon.AddUObject(this, &ThisClass::OnEquipWeapon);
		Player->GetPackageComponent()->OnUnEquipWeapon.AddUObject(this, &UPackageUserWidget::OnUnEquipWeapon);
		
		//创建模特角色
		if (!PlayerModelActor)
		{
			TSubclassOf<APlayerModelActor> ModelClass = LoadClass<APlayerModelActor>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/DeadZone/Blueprints/Player/BP_PlayerModel.BP_PlayerModel_C'"));
			PlayerModelActor = GetWorld()->SpawnActor<APlayerModelActor>(ModelClass, FVector(0, 0, 100000), FRotator::ZeroRotator);
			PlayerModelActor->SetPlayer(Player);
		}
		if (PlayerModelActor)
		{
			PlayerModelActor->SetActorHiddenInGame(false);
		}
	}
}

void UPackageUserWidget::NativeDestruct()
{
	if (ADzPlayerCharacter* Player = Cast<ADzPlayerCharacter>(GetOwningPlayerPawn()))
	{
		for (auto Item  : Player->GetPackageComponent()->GetNearItemActors())
		{
			NearInventory->RemoveNearItemActor(Item);
		}
	}
}

bool UPackageUserWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	if (ADzPlayerCharacter* Player = Cast<ADzPlayerCharacter>(GetOwningPlayerPawn()))
	{
		if (UItemFrameWidget* ItemWidget = Cast<UItemFrameWidget>(InOperation->Payload))
		{
			if (!ItemWidget->GetInventoryWidget()->IsNear())
			{
				Player->GetPackageComponent()->RemoveItemFromPackage(ItemWidget->GetActorID(), ItemWidget->GetAmount(),false);
				ItemWidget->RemoveItem();
				return true;
			}
				
		}
	}
	return true;
	
}

bool UPackageUserWidget::OnAddItemToPackage(int32 ID, int32 Amount)
{
	if (PocketInventory->AddItem(ID, Amount))
	{
		return true;
	}
	if (PackageInventory->IsVisible())
	{
		return PackageInventory->AddItem(ID, Amount);
	}
	return false;
}

bool UPackageUserWidget::OnAddItemToStorage(int32 ID, int32 Amount)
{
	return TargetInventory->AddItem(ID, Amount);
}

void UPackageUserWidget::OnInventoryChanged(TMap<int, FInventorySlot> Items)
{
	TargetInventory->InitItemGrid(Items);
}

void UPackageUserWidget::OnInventorySaved(ASceneStorageActor* StorageActor)
{
	TargetInventory->Save(StorageActor);
}

void UPackageUserWidget::OnAddNearItemActor(ASceneItemActor* ItemActor)
{
	NearInventory->AddNearItemActor(ItemActor);
}

void UPackageUserWidget::OnRemoveNearItemActor(ASceneItemActor* ItemActor)
{
	NearInventory->RemoveNearItemActor(ItemActor);
}

void UPackageUserWidget::OnPutOnEquipment(EEquipmentType EquipmentType, int32 ID)
{
	EquipmentWidget->PutOnEquipment(EquipmentType, ID);
}

void UPackageUserWidget::OnTakeoffEquipment(EEquipmentType EquipmentType, int32 ID)
{
	if (ADzPlayerCharacter* Player = Cast<ADzPlayerCharacter>(GetOwningPlayerPawn()))
	{
		bool bSuccess = Player->GetPackageComponent()->AddItemAuto(ID);
		if (!bSuccess)
		{
			Player->GetPackageComponent()->RemoveItemFromPackage(ID, 1, false);
		}
	}
	EquipmentWidget->TakeoffEquipment(EquipmentType, ID);
	
	
}

void UPackageUserWidget::OnEquipWeapon(int32 ID)
{
	EquipmentWidget->EquipWeapon(ID);
}

void UPackageUserWidget::OnUnEquipWeapon(int32 ID)
{
	EquipmentWidget->UnEquipWeapon();
}

int32 UPackageUserWidget::OnConsumeAmmo(int32 BulletID, int32 NeededAmount)
{
	int32 PocketAmount = PocketInventory->ConsumeItemsByID(BulletID, NeededAmount);
	int32 PackageAmount = PackageInventory->ConsumeItemsByID(BulletID, NeededAmount-PocketAmount);
	return PackageAmount+PocketAmount;
}


void UPackageUserWidget::ChangeTargetInventory()
{
	if (!TargetInventory->IsVisible())
	{
		TargetInventory->SetVisibility(ESlateVisibility::Visible);
		NearInventory->SetVisibility(ESlateVisibility::Hidden);
	} else
	{
		TargetInventory->SetVisibility(ESlateVisibility::Hidden);
		NearInventory->SetVisibility(ESlateVisibility::Visible);
	}
}
