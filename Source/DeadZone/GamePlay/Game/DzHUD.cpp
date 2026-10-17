// Fill out your copyright notice in the Description page of Project Settings.


#include "DzHUD.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "DeadZone/Character/DzCharacterBase.h"
#include "DeadZone/Player/DzPlayerCharacter.h"
#include "DeadZone/UI/Game/GameFeatureUserWidget.h"
#include "DeadZone/UI/Game/PackageUserWidget.h"
#include "DeadZone/UI/Game/TooltipWidget.h"
#include "Engine/Canvas.h"

class UCanvasPanelSlot;

void ADzHUD::BeginPlay()
{
	Super::BeginPlay();
	//显示游戏的主UI
	TSubclassOf<UGameFeatureUserWidget> GFWidgetClass = LoadClass<UGameFeatureUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/DeadZone/UI/Game/UMG_GameFeature.UMG_GameFeature_C'"));
	GameFeatureUserWidget = CreateWidget<UGameFeatureUserWidget>(GetOwningPlayerController(), GFWidgetClass);
	if (GameFeatureUserWidget)
	{
		GameFeatureUserWidget->AddToViewport();
		//关联角色
		GameFeatureUserWidget->SetGamePlayerPawn(Cast<ADzPlayerCharacter>(GetOwningPawn()));
	}
	
	TSubclassOf<UTooltipWidget> ToolWidgetClass = LoadClass<UTooltipWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/DeadZone/UI/Game/WBP_Tooltip.WBP_Tooltip_C'"));
	TooltipWidget = CreateWidget<UTooltipWidget>(GetWorld(), ToolWidgetClass);
	if (TooltipWidget)
	{
		TooltipWidget->AddToViewport(99); // 保证绘制在最顶层
		TooltipWidget->SetRenderTransformPivot(FVector2D(0.0f, 0.0f));
		TooltipWidget->SetRenderScale(FVector2D(0.3f, 0.3f));
	}
	

	//设置下输入模式
	FInputModeGameOnly InputMode;
	GetOwningPlayerController()->SetInputMode(InputMode);
	GameFeatureUserWidget->PlayAnimation(GameFeatureUserWidget->FadeIn);
	
	TSubclassOf<UPackageUserWidget> WidgetClass = LoadClass<UPackageUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/DeadZone/UI/Game/UMG_Package.UMG_Package_C'"));
	PackageUserWidget = CreateWidget<UPackageUserWidget>(GetOwningPlayerController(), WidgetClass);
	PackageUserWidget->AddToViewport();
	PackageUserWidget->SetVisibility(ESlateVisibility::Hidden);
}

void ADzHUD::DrawHUD()
{
	Super::DrawHUD();
	
	//绘制准星
	if (!Player)
	{
		Player = Cast<ADzCharacterBase>(GetOwningPawn());
		return;
	}

	if (Player->IsIronsight())
	{
		float AimLineLen = 8.f;
		float AimLineOffset = 10.f;
		
		float CenterX = Canvas->SizeX / 2.f;
		float CenterY = Canvas->SizeY / 2.f;
		
		// 四周
		DrawLine(CenterX, CenterY - AimLineLen - AimLineOffset, CenterX, CenterY - AimLineOffset, FLinearColor::Red, 2);
		DrawLine(CenterX, CenterY + AimLineLen + AimLineOffset, CenterX, CenterY + AimLineOffset, FLinearColor::Red, 2);
		
		DrawLine(CenterX - AimLineOffset - AimLineLen, CenterY, CenterX - AimLineOffset, CenterY, FLinearColor::Red, 2);
		DrawLine(CenterX + AimLineOffset + AimLineLen, CenterY, CenterX + AimLineOffset, CenterY, FLinearColor::Red, 2);
		
		float DotSize = 2.f; // 中心点的大小（像素）
		// 为了让点严格居中，绘制起点需要向左和向上偏移点大小的一半
		DrawRect(
			FLinearColor::Red, 
			CenterX - (DotSize / 2.f), 
			CenterY - (DotSize / 2.f), 
			DotSize, 
			DotSize
		);
	}
	
}

void ADzHUD::TogglePackageUI()
{
	if (PackageUserWidget)
	{
		/*if (PackageUserWidget->IsInViewport())
		{
			//PackageUserWidget->RemoveFromParent();
			// 设置窗口聚焦
			//当UI关闭时，需要聚焦到游戏视口，否则将需要点击一下鼠标，视口才能获取鼠标事件
			FInputModeGameOnly InputMode;
			GetOwningPlayerController()->SetInputMode(InputMode);
		}
		else
		{
			PackageUserWidget->AddToViewport();
			// 设置窗口聚焦
			//当打开UI的时候需要将窗口聚焦到UI上，否则鼠标无法直接操作，需要额外点击一次
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(PackageUserWidget->TakeWidget());//鼠标聚焦
			GetOwningPlayerController()->SetInputMode(InputMode);
			if (GEngine && GEngine->GameViewport)
			{
				FVector2D ViewportSize;
				GEngine->GameViewport->GetViewportSize(ViewportSize);

				int32 CenterX = FMath::RoundToInt(ViewportSize.X * 0.5f);
				int32 CenterY = FMath::RoundToInt(ViewportSize.Y * 0.5f);
				
				GetOwningPlayerController()->SetMouseLocation(CenterX, CenterY);
			}
		}
		//显示鼠标光标
		GetOwningPlayerController()->bShowMouseCursor = PackageUserWidget->IsInViewport();*/
		if (PackageUserWidget->IsVisible())
		{
			// ----- 关闭背包 UI -----
			PackageUserWidget->SetVisibility(ESlateVisibility::Hidden);

			// 切换回纯游戏输入模式
			FInputModeGameOnly InputMode;
			GetOwningPlayerController()->SetInputMode(InputMode);
			GetOwningPlayerController()->bShowMouseCursor = false;
		}
		else
		{
			// ----- 打开背包 UI -----
			PackageUserWidget->SetVisibility(ESlateVisibility::Visible);

			// 切换为游戏与 UI 混合模式并聚焦
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(PackageUserWidget->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			GetOwningPlayerController()->SetInputMode(InputMode);
			GetOwningPlayerController()->bShowMouseCursor = true;

			// 居中鼠标
			if (GEngine && GEngine->GameViewport)
			{
				FVector2D ViewportSize;
				GEngine->GameViewport->GetViewportSize(ViewportSize);
				GetOwningPlayerController()->SetMouseLocation(
					FMath::RoundToInt(ViewportSize.X * 0.5f), 
					FMath::RoundToInt(ViewportSize.Y * 0.5f)
				);
			}
		}
	}
}

void ADzHUD::ShowItemTooltip(int32 Row, int32 Column)
{
	if (TooltipWidget)
	{
		const float AlignmentY = Row >= 2 ? 0.275f : 0.0f;
		const float AlignmentX = Column >= 2 ? 0.275f : 0.0f;
		TooltipWidget->SetAlignmentInViewport(
			FVector2D(AlignmentX, AlignmentY)
		);
		
		TooltipWidget->SetVisibility(
			ESlateVisibility::HitTestInvisible
		);
	}
	
}


void ADzHUD::UpdateItemTooltipPosition(FVector2D MousePosition)
{
	if (TooltipWidget)
	{
		FVector2D PixelPosition;
		FVector2D ViewportPosition;

		USlateBlueprintLibrary::AbsoluteToViewport(
			GetWorld(),
			MousePosition,
			PixelPosition,
			ViewportPosition
		);

		TooltipWidget->SetPositionInViewport(
			ViewportPosition,
			false
		);
	}
}

void ADzHUD::UpdateItemTooltipInfo(int32 ID, bool IsNear)
{
	if (TooltipWidget)
	{
		TooltipWidget->UpdateInfo(ID, IsNear);
	}
}

void ADzHUD::UpdateItemTooltipInfo(int32 ID)
{
	if (TooltipWidget)
	{
		TooltipWidget->UpdateInfo(ID);
	}
}


void ADzHUD::HideItemTooltip()
{
	if (TooltipWidget)
	{
		TooltipWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

bool ADzHUD::IsItemTooltipVisible()
{
	return TooltipWidget->IsVisible();
}

void ADzHUD::ChangeTargetInventory()
{
	PackageUserWidget->ChangeTargetInventory();
}

void ADzHUD::FadeOutAnim()
{
	GameFeatureUserWidget->PlayAnimation(GameFeatureUserWidget->FadeOut);
}
