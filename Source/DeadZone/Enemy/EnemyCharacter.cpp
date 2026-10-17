// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "EnemyController.h"
#include "DeadZone/Components/PackageComponent.h"
#include "DeadZone/Core/DzGameDefines.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//设置默认的AI控制器
	AIControllerClass = AEnemyController::StaticClass();
	//修改AI控制器的启动方式
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	TeamType = ETeamType::ETT_Enemy;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	PackageComponent->PutOnEquipment(FMath::RandBool() ? 5 : 6);
	PackageComponent->EquipWeapon(101);
}

void AEnemyCharacter::OnDead()
{
	Super::OnDead();
	if (AEnemyController* EnemyController = Cast<AEnemyController>(GetController()))
	{
		EnemyController->NotifyDead();
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

