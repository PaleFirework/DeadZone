// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeadZone/Components/PackageComponent.h"
#include "EquipmentFrameWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class DEADZONE_API UEquipmentFrameWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativePreConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;	
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	UEquipmentFrameWidget* CopySelf();
	
public:
	EEquipmentType GetEquipmentType() const { return EquipmentType; }
	
	void UpdateIcon(int32 ID);
	
protected:
	bool bIsDragging = false;
	
	UPROPERTY(EditAnywhere)
	EEquipmentType EquipmentType;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> IconImage;
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	TObjectPtr<UImage> Icon;
	int32 EquipmentID;
};
