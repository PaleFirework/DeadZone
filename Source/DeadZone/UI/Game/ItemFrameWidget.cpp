// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemFrameWidget.h"

#include "CommonTextBlock.h"
#include "InventoryWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/UniformGridSlot.h"
#include "DeadZone/Components/PackageComponent.h"
#include "DeadZone/Core/DzGameDefines.h"
#include "DeadZone/GamePlay/Game/DzHUD.h"
#include "DeadZone/Player/DzPlayerCharacter.h"
#include "DeadZone/Sence/SceneItemActor.h"
#include "DeadZone/Subsystem/PropsSubsystem.h"

//返回去的FReply：所有的UI系统，在处理输入事件的时候（鼠标），需要管理分发机制。当一个触摸或是点击事件产生，需要找到响应点击事件的控件。
//增加的返回可以告知输入的事件系统，此次的输入事件是否要被消耗掉。如果消耗，则不会继续向下传递，不消耗则继续向下传递
FReply UItemFrameWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	
	if (!IconImage->IsVisible())
	{
		return FReply::Handled();
	}
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		if (ADzPlayerCharacter* Player = Cast<ADzPlayerCharacter>(GetOwningPlayerPawn()))
		{
			if (InventoryWidget->IsNear() || InventoryWidget->IsStorage())
			{
				if (Player->GetPackageComponent()->AddItemAuto(ActorID, Amount))
				{
					Player->GetPackageComponent()->DestroyItemFromNear(BindSceneItemActor);
					if (InventoryWidget->IsStorage())
					{
						RemoveItem();
					}
				}
			} else 
			{
				if (!Player->GetIsStorage())
				{
					Player->GetPackageComponent()->RemoveItemFromPackage(ActorID, Amount, false);
					RemoveItem();
				} else
				{
					if (Player->GetPackageComponent()->AddItemToStorage(ActorID, Amount))
					{
						RemoveItem();
					}
				}
			}
			
		}
		return FReply::Handled();
	}
	//开启拖拽的检测  当输入符合要求是，开启当前控件的拖拽检测，当有拖拽检测，才会产生拖拽事件（由触摸开始或由鼠标开始）
	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

FReply UItemFrameWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		return FReply::Handled();
	}
	
	if (ActorID >= CONSUMABLE_INDEX)
	{
		// TODO 添加使用的效果
		return FReply::Handled();
	}
	
	if (!bIsDragging && IconImage && IconImage->IsVisible())
	{
		if (ADzPlayerCharacter* Player = Cast<ADzPlayerCharacter>(GetOwningPlayerPawn()))
		{
			Player->GetPackageComponent()->PutOnEquipment(ActorID);
			if (InventoryWidget->IsNear())
			{
				Player->GetPackageComponent()->DestroyItemFromNear(BindSceneItemActor);
			}
			RemoveItem();

			FTimerDelegate TimerDelegate;
			TimerDelegate.BindLambda([this]()
			{
				if (ADzHUD* HUD = Cast<ADzHUD>(GetOwningPlayer()->GetHUD()))
				{
					if (IsEmpty())
					{
						HUD->HideItemTooltip();
					} else
					{
						HUD->UpdateItemTooltipInfo(ActorID, InventoryWidget->IsNear()||InventoryWidget->IsStorage());
					}
				}
			});
			GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);
			
		}
	}
	bIsDragging = false;

	return FReply::Handled();
}

void UItemFrameWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                            UDragDropOperation*& OutOperation)
{
	bIsDragging = true;

	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(
		UDragDropOperation::StaticClass()
	);

	if (!OutOperation)
	{
		bIsDragging = false;
		return;
	}

	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	//设置拖拽的Payload
	OutOperation->Payload = this;//告知我们拖拽的这次操作，要消耗的是哪个控件
	//设置拖拽的虚拟体(拖拽参考)
	OutOperation->DefaultDragVisual = CopySelf();
	//调整鼠标点击的位置
	OutOperation->Pivot = EDragPivot::MouseDown;
}

bool UItemFrameWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	if (UItemFrameWidget* SourceWidget = Cast<UItemFrameWidget>(InOperation->Payload))
	{
		if (SourceWidget == this) return true; // 避免拖给自己
		if (!SourceWidget->IconImage->IsVisible()) return true;
		if (SourceWidget->InventoryWidget->IsNear() && !InventoryWidget->IsNear())
		{
			const FPropsBase* PropsBase = GetWorld()->GetGameInstance()->GetSubsystem<UPropsSubsystem>()->GetProps(SourceWidget->ActorID);
			if (!PropsBase || !PropsBase->bCanAddToPackage) return true;
		}
		
		FSlateBrush SourceBrush = SourceWidget->IconImage->GetBrush();
		FSlateBrush TargetBrush = IconImage->GetBrush();
		
		// 交换
		SourceWidget->IconImage->SetBrush(TargetBrush);
		IconImage->SetBrush(SourceBrush);
		
		TObjectPtr<ASceneItemActor> TempSceneItemActor = SourceWidget->BindSceneItemActor;
		SourceWidget->BindSceneItemActor = BindSceneItemActor;
		BindSceneItemActor = TempSceneItemActor;
		
		int32 TempActorID = SourceWidget->ActorID;
		SourceWidget->ActorID = ActorID;
		ActorID = TempActorID;
		
		int32 TempAmount = SourceWidget->Amount;
		SourceWidget->SetAmount(Amount);
		SetAmount(TempAmount);
		
		if (!IconImage->IsVisible())
		{
			SourceWidget->IconImage->SetVisibility(ESlateVisibility::Hidden);
		}
		IconImage->SetVisibility(ESlateVisibility::Visible);
		
		// 是不是来自附近
		if (SourceWidget->InventoryWidget->IsNear() && !InventoryWidget->IsNear())
		{
			if (ADzPlayerCharacter* Player = Cast<ADzPlayerCharacter>(GetOwningPlayerPawn()))
			{
				Player->GetPackageComponent()->DestroyItemFromNear(BindSceneItemActor);
				SourceWidget->BindSceneItemActor = Player->GetPackageComponent()->RemoveItemFromPackage(SourceWidget->ActorID, SourceWidget->Amount);
				return true;
			}
		}
		// 是不是来自背包
		if (!SourceWidget->InventoryWidget->IsNear() && InventoryWidget->IsNear())
		{
			if (ADzPlayerCharacter* Player = Cast<ADzPlayerCharacter>(GetOwningPlayerPawn()))
			{
				Player->GetPackageComponent()->DestroyItemFromNear(SourceWidget->BindSceneItemActor);
				BindSceneItemActor = Player->GetPackageComponent()->RemoveItemFromPackage(ActorID, Amount);
				return true;
			}
		}
		
		return true;
	}
    
	return false;
}

FReply UItemFrameWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);
	APlayerController* PC = GetOwningPlayer();
	ADzHUD* Hud = PC ? Cast<ADzHUD>(PC->GetHUD()) : nullptr;
	if (!Hud) return FReply::Unhandled();

	// 如果格子是空的，直接隐藏 Tooltip 并退出
	if (IsEmpty())
	{
		Hud->HideItemTooltip();
		return FReply::Unhandled();
	}

	// 更新 Tooltip 位置（跟随鼠标）
	const FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
	Hud->UpdateItemTooltipPosition(MousePosition);
	
	if (!Hud->IsItemTooltipVisible())
	{
		if (UUniformGridSlot* UniformSlot = Cast<UUniformGridSlot>(this->Slot))
		{
			const bool bIsNear = InventoryWidget ? InventoryWidget->IsNear() : false;
			const bool bIsStorage = InventoryWidget ? InventoryWidget->IsStorage() : false;
            
			Hud->UpdateItemTooltipInfo(ActorID, bIsNear||bIsStorage);
			Hud->ShowItemTooltip(UniformSlot->GetRow(), UniformSlot->GetColumn());
		}
	}

	return FReply::Unhandled();
}


void UItemFrameWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ADzHUD* HUD = Cast<ADzHUD>(PC->GetHUD()))
		{
			HUD->HideItemTooltip();
		}
	}
}

UItemFrameWidget* UItemFrameWidget::CopySelf()
{
	if (ADzHUD* Hud = Cast<ADzHUD>(GetOwningPlayer()->GetHUD()))
	{
		UItemFrameWidget* CopyObject = Hud->GetSingleObject<UItemFrameWidget>(GetClass());
		
		CopyObject->IconImage->SetBrush(IconImage->GetBrush());
		CopyObject->SetAmount(Amount);
		CopyObject->IconImage->SetVisibility(ESlateVisibility::Visible);
		FLinearColor Color = IconImage->GetColorAndOpacity();
		Color.A = 0.75f;
		CopyObject->IconImage->SetColorAndOpacity(Color);
		
		return CopyObject;
	}
	return nullptr;
}

void UItemFrameWidget::SetItem(int32 ID, int32 Count)
{
	const FPropsBase* PropsBase = GetWorld()->GetGameInstance()->GetSubsystem<UPropsSubsystem>()->GetProps(ID);
	if (PropsBase)
	{
		IconImage->SetBrushFromTexture(PropsBase->Icon);
		IconImage->SetVisibility(ESlateVisibility::Visible);
		BindSceneItemActor = nullptr;
		ActorID = ID;
		SetAmount(Count);
	}
}

void UItemFrameWidget::SetAmount(int32 Count)
{
	AmountText->SetText(FText::AsNumber(Count));
	Amount = Count;

	if (Amount >= 2)
	{
		AmountText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		AmountText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UItemFrameWidget::SetItem(TObjectPtr<ASceneItemActor> SceneItemActor)
{
	if (!IsValid(SceneItemActor))//防止道具被其他人捡走了！
	{
		return;
	}
	const FPropsBase* PropsBase = GetWorld()->GetGameInstance()->GetSubsystem<UPropsSubsystem>()->GetProps(SceneItemActor->GetID());
	if (PropsBase)
	{
		IconImage->SetBrushFromTexture(PropsBase->Icon);
		IconImage->SetVisibility(ESlateVisibility::Visible);
		BindSceneItemActor = SceneItemActor;
		ActorID = SceneItemActor->GetID();
		SetAmount(SceneItemActor->GetAmount());
	}
}

void UItemFrameWidget::RemoveItem()
{
	IconImage->SetVisibility(ESlateVisibility::Hidden);
	IconImage->SetBrushFromTexture(nullptr);
	BindSceneItemActor = nullptr;
	ActorID = -1;
	SetAmount(0);
	bIsDragging = false;
}

void UItemFrameWidget::SetInventoryWidget(UInventoryWidget* Widget)
{
	InventoryWidget = Widget;
}

void UItemFrameWidget::InitItemFrame(int32 ID, int32 Count)
{
	SetItem(ID);
	SetAmount(Count);
}

