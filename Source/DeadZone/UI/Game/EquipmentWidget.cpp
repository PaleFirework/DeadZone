// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentWidget.h"

#include "EquipmentFrameWidget.h"
#include "InventoryWidget.h"
#include "ItemFrameWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/VerticalBox.h"
#include "DeadZone/Components/PackageComponent.h"
#include "DeadZone/Core/DzGameDefines.h"
#include "DeadZone/GamePlay/Game/DzHUD.h"
#include "DeadZone/Player/DzPlayerCharacter.h"

bool UEquipmentWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                    UDragDropOperation* InOperation)
{
	if (ADzPlayerCharacter* Player = Cast<ADzPlayerCharacter>(GetOwningPlayerPawn()))
	{
		if (UItemFrameWidget* ItemWidget = Cast<UItemFrameWidget>(InOperation->Payload))
		{
			if (ItemWidget->GetActorID() >= CONSUMABLE_INDEX)
			{
				return true;
			}
			if (!ItemWidget->GetInventoryWidget()->IsNear())
			{
				Player->GetPackageComponent()->PutOnEquipment(ItemWidget->GetActorID());
				ItemWidget->RemoveItem();
			} else
			{
				Player->GetPackageComponent()->PutOnEquipment(ItemWidget->GetActorID());
				Player->GetPackageComponent()->DestroyItemFromNear(ItemWidget->GetSceneItemActor());
			}
		}
	}
	return true;
}


void UEquipmentWidget::PutOnEquipment(EEquipmentType EquipmentType, int32 ID)
{
	for (int i = 0; i < ArmorPanel->GetChildrenCount(); ++i)
	{
		if (UEquipmentFrameWidget* EquipmentFrameWidget = Cast<UEquipmentFrameWidget>(ArmorPanel->GetChildAt(i)))
		{
			if (EquipmentFrameWidget->GetEquipmentType() == EquipmentType)
			{
				EquipmentFrameWidget->UpdateIcon(ID);
				if (EquipmentType == EEquipmentType::EST_Package)
				{
					if (ADzPlayerCharacter* Player = Cast<ADzPlayerCharacter>(GetOwningPlayerPawn()))
					{
						Player->GetPackageComponent()->ShowPackageInventory();
					}
				}
				break;
			}	
		}
	}
}

void UEquipmentWidget::TakeoffEquipment(EEquipmentType EquipmentType, int32 ID)
{
	for (int i = 0; i < ArmorPanel->GetChildrenCount(); ++i)
	{
		if (UEquipmentFrameWidget* EquipmentFrameWidget = Cast<UEquipmentFrameWidget>(ArmorPanel->GetChildAt(i)))
		{
			if (EquipmentFrameWidget->GetEquipmentType() == EquipmentType)
			{
				EquipmentFrameWidget->UpdateIcon(-1);
				if (EquipmentType == EEquipmentType::EST_Package)
				{
					if (ADzPlayerCharacter* Player = Cast<ADzPlayerCharacter>(GetOwningPlayerPawn()))
					{
						Player->GetPackageComponent()->UnShowPackageInventory();
					}
				}
				break;
			}	
		}
	}
}

void UEquipmentWidget::EquipWeapon(int32 ID)
{
	WeaponFrame->UpdateIcon(ID);
}

void UEquipmentWidget::UnEquipWeapon()
{
	WeaponFrame->UpdateIcon(-1);
}
