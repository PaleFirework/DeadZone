// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentWidget.generated.h"

class UEquipmentFrameWidget;
enum class EEquipmentType : uint8;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class DEADZONE_API UEquipmentWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	


public:
	void PutOnEquipment(EEquipmentType EquipmentType, int32 ID);
	void TakeoffEquipment(EEquipmentType EquipmentType, int32 ID);
	
	void EquipWeapon(int32 ID);
	void UnEquipWeapon();

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> ArmorPanel;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEquipmentFrameWidget> WeaponFrame;
};
