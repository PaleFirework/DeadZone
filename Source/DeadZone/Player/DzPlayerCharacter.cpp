// Fill out your copyright notice in the Description page of Project Settings.


#include "DzPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "DeadZone/Components/PackageComponent.h"
#include "DeadZone/Core/DzGameDefines.h"
#include "DeadZone/GamePlay/Game/DzHUD.h"
#include "DeadZone/GameSettings/DzGameUserSettings.h"
#include "DeadZone/SaveGame/KeyMappingSaveGame.h"
#include "DeadZone/Sence/SceneItemActor.h"
#include "DeadZone/Sence/SceneStorageActor.h"
#include "DeadZone/UI/MainMenu/KeySettingWidget.h"
#include "DeadZone/Weapon/WeaponActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

#define ADD_KEY_IMC(KeyEventName) if (InputMappingDT->GetRowMap().Contains(KeyEventName))\
{\
FKeyInfoHeader* InfoHeader = reinterpret_cast<FKeyInfoHeader*>(InputMappingDT->GetRowMap()[KeyEventName]);\
FKey Key = GetCustomKey(KeyEventName);\
if (!Key.IsValid())\
{\
Key = InfoHeader->DefaultKey;\
}\
IMC_Player->MapKey(InfoHeader->InputAction, Key);\
}


// Sets default values
ADzPlayerCharacter::ADzPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	//相机吊臂跟随控制器旋转
	SpringArmComponent->bUsePawnControlRotation = true;
	
	NearSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("NearSphereComponent"));
	NearSphereComponent->SetupAttachment(RootComponent);
	NearSphereComponent->SetSphereRadius(500.f);
	
	/*NearSphereComponent1 = CreateDefaultSubobject<USphereComponent>(TEXT("NearSphereComponent1"));
	NearSphereComponent1->SetupAttachment(RootComponent);
	NearSphereComponent1->SetSphereRadius(500.f);*/
	
	//绑定通知
	NearSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnNearSphereBeginOverlap);
	NearSphereComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnNearSphereEndOverlap);
	

}

// Called when the game starts or when spawned
void ADzPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetupInputContext();
}

void ADzPlayerCharacter::SetupInputContext()
{
	//装在输入映射关系
	if (APlayerController* Pc = Cast<APlayerController>(GetController()))
	{
		if (!IMC_Player)
		{
			IMC_Player = LoadObject<UInputMappingContext>(this, TEXT("/Game/DeadZone/Input/IMC_Player"));
		}
		
		//通过数据表格来完成数据的装载
		if (!InputMappingDT)
		{
			InputMappingDT = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/DeadZone/Data/KeyMapping/DT_KeyMapping.DT_KeyMapping'"));
		}

		if (!InputMappingDT)
		{
			return;
		}
		
		//清理映射表
		IMC_Player->UnmapAll();
		
		
		ADD_KEY_IMC(TEXT("Jump"));
		ADD_KEY_IMC(TEXT("Fire"));
		ADD_KEY_IMC(TEXT("Reload"));
		ADD_KEY_IMC(TEXT("Sprint"));
		ADD_KEY_IMC(TEXT("Crouch"));
		ADD_KEY_IMC(TEXT("Interact"));
		ADD_KEY_IMC(TEXT("Package"));
		ADD_KEY_IMC(TEXT("Ironsight"));
		ADD_KEY_IMC(TEXT("Look"));
		
		AddAxisKeyImc(TEXT("MoveForward"), false, EAxis::Y);		
		AddAxisKeyImc(TEXT("MoveBack"), true, EAxis::Y);		
		AddAxisKeyImc(TEXT("MoveRight"));
		AddAxisKeyImc(TEXT("MoveLeft"), true);
		
		KeyMappingSaveGame = nullptr;
		
		//获取到增强输入子系统
		UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Pc->GetLocalPlayer());
		//装载输入映射
		LocalPlayerSubsystem->AddMappingContext(IMC_Player, 0);
	}
}

UInputAction* ADzPlayerCharacter::GetInputActionFromDataTable(FName RowName)
{
	if (!InputMappingDT)
	{
		InputMappingDT = LoadObject<UDataTable>(this,  TEXT("/Game/DeadZone/Data/KeyMapping/DT_KeyMapping"));
	}

	if (InputMappingDT && InputMappingDT->GetRowMap().Contains(RowName))
	{
		FKeyInfoHeader* KeyInfoHeader = reinterpret_cast<FKeyInfoHeader*>(InputMappingDT->GetRowMap()[RowName]);
		return KeyInfoHeader->InputAction;
	}
	return nullptr;
}

FKey ADzPlayerCharacter::GetCustomKey(FName KeyEventName)
{
	FKey Key;
	if (!KeyMappingSaveGame && !UGameplayStatics::DoesSaveGameExist(CUSTOM_KEY_SAVE_NAME, 0))
	{
		return Key;
	}
	if (!KeyMappingSaveGame)
	{
		KeyMappingSaveGame = Cast<UKeyMappingSaveGame>(UGameplayStatics::LoadGameFromSlot(CUSTOM_KEY_SAVE_NAME, 0));
	}
	if (KeyMappingSaveGame->CustomKeyMap.Contains(KeyEventName))
	{
		return KeyMappingSaveGame->CustomKeyMap[KeyEventName];
	}	
	return Key;
}

void ADzPlayerCharacter::AddAxisKeyImc(FName RowName, bool bNegate, EAxis::Type Axis)
{
	if (!IMC_Player || !InputMappingDT)
	{
		return;
	}
	
	if (InputMappingDT->GetRowMap().Contains(RowName))
	{
		FKeyInfoHeader* InfoHeader = reinterpret_cast<FKeyInfoHeader*>(InputMappingDT->GetRowMap()[RowName]);
		FKey Key = GetCustomKey(RowName);
		if (!Key.IsValid())
		{
			Key = InfoHeader->DefaultKey;
		}
		FEnhancedActionKeyMapping& KeyMapping = IMC_Player->MapKey(InfoHeader->InputAction, Key);
		//添加修改器
		if (bNegate)
		{
			KeyMapping.Modifiers.Add(NewObject<UInputModifierNegate>(IMC_Player));
		}
		if (Axis == EAxis::Y)
		{
			KeyMapping.Modifiers.Add(NewObject<UInputModifierSwizzleAxis>(IMC_Player));
		}
	}
}

void ADzPlayerCharacter::OnMove(const FInputActionValue& InputActionValue)
{
	FVector2D InputValue = InputActionValue.Get<FVector2D>();

	//朝着视口的正方向和右方向移动
	FRotator NewRot(0, GetControlRotation().Yaw, 0);
	
	//基于旋转获取方向轴  直接转换为四元数
	FQuat Quat = NewRot.Quaternion();
	AddMovementInput(Quat.GetForwardVector(), InputValue.Y);
	AddMovementInput(Quat.GetRightVector(), InputValue.X);
}

void ADzPlayerCharacter::OnLook(const FInputActionValue& InputActionValue)
{
	FVector2D InputValue = InputActionValue.Get<FVector2D>();
	//获取数据标速
	float MouseSpeed = 30;
	//从配置中读取速度值
	if (UDzGameUserSettings* GameUserSettings = Cast<UDzGameUserSettings>(UGameUserSettings::GetGameUserSettings()))
	{
		MouseSpeed = GameUserSettings->GetMouseSpeed();
	}
	
	//应用到控制器的Pitch和Yaw
	AddControllerPitchInput(InputValue.Y * GetWorld()->GetDeltaSeconds()* -MouseSpeed);
	AddControllerYawInput(InputValue.X * GetWorld()->GetDeltaSeconds() * MouseSpeed);
}

void ADzPlayerCharacter::OnNearSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASceneItemActor* ItemActor = Cast<ASceneItemActor>(OtherActor))
	{
		ItemActor->SetItemOutline(true);
		if (PackageComponent)
		{
			PackageComponent->AddNearItemActor(ItemActor);
		}
	}
	if (ASceneStorageActor* StorageActor = Cast<ASceneStorageActor>(OtherActor))
	{
		StorageActor->SetItemOutline(true);
		if (PackageComponent)
		{
			PackageComponent->AddNearStorageActor(StorageActor);
		}
	}
}

void ADzPlayerCharacter::OnNearSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ASceneItemActor* ItemActor = Cast<ASceneItemActor>(OtherActor))
	{
		ItemActor->SetItemOutline(false);
		if (PackageComponent)
		{
			PackageComponent->RemoveNearItemActor(ItemActor);
		}
	}
	if (ASceneStorageActor* StorageActor = Cast<ASceneStorageActor>(OtherActor))
	{
		StorageActor->SetItemOutline(false);
		if (PackageComponent)
		{
			PackageComponent->RemoveNearStorageActor(StorageActor);
		}
	}
}

void ADzPlayerCharacter::TogglePackage()
{
	if (!IsStorage)
	{
		TogglePackageUI();
	} 
}

void ADzPlayerCharacter::TogglePackageUI()
{
	if (APlayerController* Pc = Cast<APlayerController>(GetController()))
	{
		if (ADzHUD* Hud = Cast<ADzHUD>(Pc->GetHUD()))
		{
			Hud->TogglePackageUI();
		}
	}
}

void ADzPlayerCharacter::Storage()
{
	if (!IsStorage)
	{
		GetCharacterMovement()->DisableMovement();
		IsStorage = true;
	}else
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		IsStorage = false;
	}
	TogglePackageUI();
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		ADzHUD* HUD = Cast<ADzHUD>(PC->GetHUD());
		if (HUD)
		{
			HUD->ChangeTargetInventory();
		}
	}
}


void ADzPlayerCharacter::Interact()
{
	if (PackageComponent)
	{
		PackageComponent->Interact();
	}
}

/*int32 ADzPlayerCharacter::ConsumeClipFromInventory(int32 AmmoItemID, int32 AmountNeeded)
{
	AWeaponActor* CurrentWeapon = GetHoldWeapon();
	if (!CurrentWeapon) return -1;
	

	int32 ClipNeeded = CurrentWeapon->GetMaxClipVolume() - CurrentWeapon->GetCurrentClip();

	
}*/


// Called every frame
void ADzPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADzPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//绑定响应Action
	if (UEnhancedInputComponent* MyEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (UInputAction* InputAction = GetInputActionFromDataTable(TEXT("Jump")))
		{
			MyEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this,&ThisClass::Jump);
		}
		if (UInputAction* InputAction = GetInputActionFromDataTable(TEXT("Package")))
		{
			MyEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this,&ThisClass::TogglePackage);
		}
		if (UInputAction* InputAction = GetInputActionFromDataTable(TEXT("Crouch")))
		{
			MyEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this,&ThisClass::DoCrouch);
		}
		if (UInputAction* InputAction = GetInputActionFromDataTable(TEXT("Fire")))
		{
			//绑定按下
			MyEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this,&ThisClass::StartFire);
			//绑定抬起
			MyEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this,&ThisClass::StopFire);
		}
		if (UInputAction* InputAction = GetInputActionFromDataTable(TEXT("Reload")))
		{
			MyEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this,&ThisClass::ReloadWeapon);
		}
		if (UInputAction* InputAction = GetInputActionFromDataTable(TEXT("Interact")))
		{
			MyEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this,&ThisClass::Interact);
		}
		if (UInputAction* InputAction = GetInputActionFromDataTable(TEXT("MoveForward")))
		{
			MyEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this,&ThisClass::OnMove);
		}
		if (UInputAction* InputAction = GetInputActionFromDataTable(TEXT("Look")))
		{
			MyEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this,&ThisClass::OnLook);
		}
		if (UInputAction* InputAction = GetInputActionFromDataTable(TEXT("Sprint")))
		{
			MyEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this,&ThisClass::StartSprint);
			MyEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this,&ThisClass::StopSprint);
		}
		if (UInputAction* InputAction = GetInputActionFromDataTable(TEXT("Ironsight")))
		{
			MyEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this,&ThisClass::StartIronsight);
			MyEnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this,&ThisClass::StopIronsight);
		}
	}
}


#undef ADD_KEY_IMC
