// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentFrameWidget.h"

#include "ItemFrameWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "DeadZone/Components/PackageComponent.h"
#include "DeadZone/GamePlay/Game/DzHUD.h"
#include "DeadZone/Player/DzPlayerCharacter.h"

void UEquipmentFrameWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

FReply UEquipmentFrameWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!IconImage->IsVisible())
	{
		return FReply::Handled();
	}
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		
		if (ADzPlayerCharacter* Player = Cast<ADzPlayerCharacter>(GetOwningPlayerPawn()))
		{
			if (EquipmentType == EEquipmentType::EST_None)
			{
				Player->GetPackageComponent()->UnEquipWeapon();
			} else
			{
				Player->GetPackageComponent()->TakeOffEquipment(EquipmentType);
			}
		}
		return FReply::Handled();
		
	}
	//开启拖拽的检测  当输入符合要求是，开启当前控件的拖拽检测，当有拖拽检测，才会产生拖拽事件（由触摸开始或由鼠标开始）
	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}


void UEquipmentFrameWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
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

FReply UEquipmentFrameWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);
	APlayerController* PC = GetOwningPlayer();
	ADzHUD* Hud = PC ? Cast<ADzHUD>(PC->GetHUD()) : nullptr;
	if (!Hud) return FReply::Unhandled();

	// 如果格子是空的，直接隐藏 Tooltip 并退出
	if (!IconImage->IsVisible())
	{
		Hud->HideItemTooltip();
		return FReply::Unhandled();
	}

	// 更新 Tooltip 位置（跟随鼠标）
	const FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
	Hud->UpdateItemTooltipPosition(MousePosition);

	// 如果当前还没显示，才去显示并更新信息（避免每帧重复触发开关逻辑）
	if (!Hud->IsItemTooltipVisible())
	{
		Hud->UpdateItemTooltipInfo(EquipmentID);
		Hud->ShowItemTooltip();
	}
	return FReply::Unhandled();
}

void UEquipmentFrameWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
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

UEquipmentFrameWidget* UEquipmentFrameWidget::CopySelf()
{
	if (ADzHUD* Hud = Cast<ADzHUD>(GetOwningPlayer()->GetHUD()))
	{
		UEquipmentFrameWidget* CopyObject = Hud->GetSingleObject<UEquipmentFrameWidget>(GetClass());
		
		CopyObject->IconImage->SetBrush(IconImage->GetBrush());
		CopyObject->IconImage->SetVisibility(ESlateVisibility::Visible);
		FLinearColor Color = IconImage->GetColorAndOpacity();
		Color.A = 0.75f;
		CopyObject->IconImage->SetColorAndOpacity(Color);
		
		return CopyObject;
	}
	return nullptr;
}


void UEquipmentFrameWidget::UpdateIcon(int32 ID)
{
	if (ID < 0)
	{
		IconImage->SetVisibility(ESlateVisibility::Hidden);
		Icon->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		IconImage->SetVisibility(ESlateVisibility::Visible);
		Icon->SetVisibility(ESlateVisibility::Hidden);
		
		const FPropsBase* PropsBase = GetWorld()->GetGameInstance()->GetSubsystem<UPropsSubsystem>()->GetProps(ID);
		if (PropsBase)
		{
			IconImage->SetBrushFromTexture(PropsBase->Icon);
		}
	}
	EquipmentID = ID;
}

