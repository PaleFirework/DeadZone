// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeyInfoWidget.generated.h"

class UButton;
class UInputKeySelector;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DEADZONE_API UKeyInfoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION()
	void OnKeySelected(FInputChord SelectedKey);
	
public:
	
	void SetKeyInfoPanel(FName InKeyEventName, FText KeyDescription, FKey DefaultKey);
	
	FName GetKeyEventName() const { return KeyEventName; }
	FKey GetKeyEventKey() const { return CurrentKey; }
	
	void ResetDefaultKey(FKey DefaultKey);
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> KeyDescriptionTextBlock;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UInputKeySelector> InputKeySelector;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> BinButton;
	
	FKey CurrentKey;
	FName KeyEventName;
};
