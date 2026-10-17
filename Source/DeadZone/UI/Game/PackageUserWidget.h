// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeadZone/Components/PackageComponent.h"
#include "PackageUserWidget.generated.h"

class APlayerModelActor;
class UEquipmentWidget;
enum class EEquipmentType : uint8;
class ASceneItemActor;
class UInventoryWidget;
/**
 * 
 */
UCLASS()
class DEADZONE_API UPackageUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void RemoveFromParent() override;
	
	void ShowPackageInventory();
	void UnShowPackageInventory();
	void ChangeTargetInventory();

protected:
	
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeDestruct() override;
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	bool OnAddItemToPackage(int32 ID, int32 Amount);
	bool OnAddItemToStorage(int32 ID, int32 Amount);

	void OnInventoryChanged(TMap<int, FInventorySlot> Items);
	void OnInventorySaved(ASceneStorageActor* StorageActor);
	
	void OnAddNearItemActor(ASceneItemActor* ItemActor);
	void OnRemoveNearItemActor(ASceneItemActor* ItemActor);
	
	void OnPutOnEquipment(EEquipmentType EquipmentType, int32 ID);
	void OnTakeoffEquipment(EEquipmentType EquipmentType, int32 ID);
	
	void OnEquipWeapon(int32 ID);
	void OnUnEquipWeapon(int32 ID);
	
	int32 OnConsumeAmmo(int32 BulletID, int32 NeededAmount);
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UInventoryWidget> PocketInventory;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UInventoryWidget> NearInventory;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UInventoryWidget> TargetInventory;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UInventoryWidget> PackageInventory;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEquipmentWidget> EquipmentWidget;
	UPROPERTY()
	TObjectPtr<APlayerModelActor> PlayerModelActor;

};
