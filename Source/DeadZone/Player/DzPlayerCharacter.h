// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/CapsuleComponent.h"
#include "DeadZone/Character/DzCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DzPlayerCharacter.generated.h"

class USphereComponent;
class UKeyMappingSaveGame;
class UCameraComponent;
class UInputMappingContext;
class USpringArmComponent;
class UInputAction;

UCLASS()
class DEADZONE_API ADzPlayerCharacter : public ADzCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADzPlayerCharacter();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void SetupInputContext();
	
	UInputAction* GetInputActionFromDataTable(FName RowName);
	FKey GetCustomKey(FName KeyEventName);
	
	void AddAxisKeyImc(FName RowName, bool bNegate = false, EAxis::Type Axis = EAxis::Type::X);
	
	void OnMove(const FInputActionValue& InputActionValue);
	void OnLook(const FInputActionValue& InputActionValue);
	
	UFUNCTION()
	void OnNearSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnNearSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void TogglePackage();
	void TogglePackageUI();
	void Interact();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	void Storage();
	
	TObjectPtr<USpringArmComponent> GetSprintArmComponent() const { return SpringArmComponent; }
	TObjectPtr<UCameraComponent> GetCameraComponent() const { return CameraComponent; }
	bool GetIsStorage() const {return IsStorage;}
	
protected:
	bool IsStorage = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> IMC_Player;
	UPROPERTY()
	TObjectPtr<UDataTable> InputMappingDT;
	UPROPERTY()
	TObjectPtr<UKeyMappingSaveGame> KeyMappingSaveGame;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> NearSphereComponent;
};
