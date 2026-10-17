// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameDelegates.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "ItemFrameWidget.generated.h"

class UCommonTextBlock;
class UInventoryWidget;
class UItemFrameWidget;
class UImage;
class ASceneItemActor;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemFrameLeftClicked, UItemFrameWidget*);
/**
 * 
 */
UCLASS()
class DEADZONE_API UItemFrameWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;	
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	UItemFrameWidget* CopySelf();
	

public:
	TObjectPtr<ASceneItemActor> GetSceneItemActor() const { return BindSceneItemActor; }
	void SetItem(TObjectPtr<ASceneItemActor> SceneItemActor);
	void SetItem(int32 ID, int32 Count=1);
	void SetAmount(int32 Count);
	int32 GetAmount() const {return Amount;};
	int32 GetActorID() const { return ActorID; }
	
	bool IsEmpty() const { return !IconImage->IsVisible(); }
	
	void RemoveItem();
	TObjectPtr<UInventoryWidget> GetInventoryWidget() const { return InventoryWidget; }
	void SetInventoryWidget(UInventoryWidget* Widget);
	void InitItemFrame(int32 ID, int32 Count);

protected:
	bool bIsDragging = false;
	int32 ActorID = -1;
	int32 Amount = 0;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	TObjectPtr<UImage> IconImage;
	UPROPERTY()
	TObjectPtr<ASceneItemActor> BindSceneItemActor;
	UPROPERTY()
	TObjectPtr<UInventoryWidget> InventoryWidget;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> AmountText;
	
public:
	FOnItemFrameLeftClicked OnItemClicked;
};
