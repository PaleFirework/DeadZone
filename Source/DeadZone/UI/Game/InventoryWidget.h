// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class ASceneStorageActor;
struct FInventorySlot;
class UItemFrameWidget;
class ASceneItemActor;
class UCommonTextBlock;
class UUniformGridPanel;
/**
 * 
 */
UCLASS()
class DEADZONE_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	void HandleItemFrameLeftClicked(UItemFrameWidget* ClickedFrame);
	virtual void NativeOnInitialized() override;

public:
	void SetText(const FText& InText);
	bool AddItem(int32 ID, int32 Amount);
	void AddNearItemActor(ASceneItemActor* ItemActor);
	void RemoveNearItemActor(ASceneItemActor* ItemActor);
	bool IsNear() {return IsNearWidget;}
	bool IsStorage() {return IsStorageWidget;}
	void SetNear(bool IsNear);
	void SetStorage(bool IsStorage);
	void InitItemGrid(TMap<int, FInventorySlot> Items);
	void Save(ASceneStorageActor* StorageActor);
	
	// 返回找到的数量
	int32 ConsumeItemsByID(int32 ID, int32 NeedAmount);
	
	UFUNCTION()
	void AddItemFrames(int32 Count = 5);
	

protected:
	UPROPERTY()
	bool IsNearWidget = false;
	UPROPERTY()
	bool IsStorageWidget = false;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemFrameWidget> ItemFrameWidgetClass;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUniformGridPanel> ItemGrid;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> TextBlock;
	
};
