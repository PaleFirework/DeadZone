// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "DzHUD.generated.h"

class UTooltipWidget;
class UGameFeatureUserWidget;
class ADzCharacterBase;
class UPackageUserWidget;
/**
 * 
 */
UCLASS()
class DEADZONE_API ADzHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;
	
public:
	
	void TogglePackageUI();
	
	template<typename T>
	T* GetSingleObject(UClass* ClassBP);
	
	void ShowItemTooltip(int32 Row=0, int32 Column=0);
	void UpdateItemTooltipPosition(FVector2D MousePosition);
	void UpdateItemTooltipInfo(int32 ID, bool IsNear);
	void UpdateItemTooltipInfo(int32 ID);
	void HideItemTooltip();
	bool IsItemTooltipVisible();
	void ChangeTargetInventory();
	
	
	UFUNCTION(BlueprintCallable)
	void FadeOutAnim();
	

protected:
	UPROPERTY()
	TObjectPtr<UPackageUserWidget> PackageUserWidget;
	UPROPERTY()
	TObjectPtr<UTooltipWidget> TooltipWidget;
	UPROPERTY()
	TMap<TObjectPtr<UClass>, TObjectPtr<UUserWidget>> SingleObjectMap;
	UPROPERTY()
	TObjectPtr<ADzCharacterBase> Player;
	UPROPERTY()
	TObjectPtr<UGameFeatureUserWidget> GameFeatureUserWidget;
};

template <typename T>
T* ADzHUD::GetSingleObject(UClass* ClassBP)
{
	if (!ClassBP->IsChildOf(T::StaticClass()))//强制要求ClassBP必须是T的类型或是子类型
	{
		return nullptr;
	}
	
	//UClass是虚幻引擎中为了实现反射，构建的一个类的数据类型
	if (SingleObjectMap.Contains(T::StaticClass()))
	{
		return Cast<T>(SingleObjectMap[T::StaticClass()]);
	}
	
	T* Object = CreateWidget<T>(GetOwningPlayerController(), ClassBP);
	
	SingleObjectMap.Add(T::StaticClass(), Object);
	
	return Object;
}