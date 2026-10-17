// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieController.h"

#include "ZombieCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"


// Sets default values
AZombieController::AZombieController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//添加感知组件
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	//绑定通知
	// PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::OnPerceptionTargetUpdate);
}

// Called when the game starts or when spawned
void AZombieController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZombieController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (AZombieCharacter* ZombieCharacter = Cast<AZombieCharacter>(InPawn))
	{
		//启动行为树
		RunBehaviorTree(ZombieCharacter->GetBehaviorTree());
		//给出生点赋值
		GetBlackboardComponent()->SetValueAsVector(TEXT("BirthPos"), InPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("NavPosition"), InPawn->GetActorLocation() + InPawn->GetActorForwardVector() * 1000);
	}
}

// Called every frame
void AZombieController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

