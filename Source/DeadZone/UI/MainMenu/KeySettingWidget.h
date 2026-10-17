// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeySettingWidget.generated.h"

class UButton;
class UVerticalBox;
class UKeyMappingSaveGame;
class UScrollBox;
class UInputAction;

USTRUCT()
struct FKeyInfoHeader : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	FText KeyDescription;
	UPROPERTY(EditAnywhere)
	FKey DefaultKey;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputAction;
};

/**
 * 
 */
UCLASS()
class DEADZONE_API UKeySettingWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	
	virtual void NativeOnInitialized() override;
	
	FKey GetCustomKey(FName KeyEventName);
	
	UFUNCTION(BlueprintCallable)
	void ResetAllKeys();
	
	
public:
	
	bool CheckKeysRepeat(const FKey& SelectedKey) const;
	void SaveUserCustomKey(FName KeyEventName, FKey Key);
	
protected:
	
	UPROPERTY()
	TObjectPtr<UDataTable> KeyMappingDT;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> KeysLeft;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> KeysRight;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> ResetButton;
	
	UPROPERTY()
	TObjectPtr<UKeyMappingSaveGame> KeyMappingSaveGame;
	
};
