// Fill out your copyright notice in the Description page of Project Settings.


#include "SceneBaseActor.h"

#include "Components/BillboardComponent.h"
#include "Components/WidgetComponent.h"


// Sets default values
ASceneBaseActor::ASceneBaseActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComp"));
	SetRootComponent(BillboardComponent);
	
	PromptWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PromptWidget"));
	PromptWidget->SetupAttachment(RootComponent);
	
	PromptWidget->SetWidgetSpace(EWidgetSpace::Screen);
	PromptWidget->SetDrawSize(FVector2D(75.0f, 75.0f));
	
	static ConstructorHelpers::FClassFinder<UUserWidget> PromptWidgetClassFinder(
		TEXT("/Game/DeadZone/UI/Game/WBP_Prompt.WBP_Prompt_C")
	);

	if (PromptWidgetClassFinder.Succeeded())
	{
		PromptWidget->SetWidgetClass(PromptWidgetClassFinder.Class);
		PromptWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
		// 默认关闭 Widget 的 Tick 和 渲染
		PromptWidget->SetComponentTickEnabled(false);
		PromptWidget->SetVisibility(false);
	}
}

// Called when the game starts or when spawned
void ASceneBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}

TObjectPtr<UStaticMeshComponent> ASceneBaseActor::GetStaticMeshComponent()
{
	if (!StaticMeshComponent)
	{
		StaticMeshComponent = NewObject<UStaticMeshComponent>(this);//这里一定要传outer，不然组件不知道他的owner是谁
		StaticMeshComponent->RegisterComponentWithWorld(GetWorld());//保证组件内可以拿到world
		//设置依附关系
		StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		//设置碰撞预设
		StaticMeshComponent->SetCollisionProfileName(TEXT("SceneItemActor"));
	}
	return StaticMeshComponent;
}

TObjectPtr<USkeletalMeshComponent> ASceneBaseActor::GetSkeletalMeshComponent()
{
	if (!SkeletalMeshComponent)
	{
		SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(this);
		SkeletalMeshComponent->RegisterComponentWithWorld(GetWorld());
		SkeletalMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		SkeletalMeshComponent->SetCollisionProfileName(TEXT("SceneItemActor"));
		SkeletalMeshComponent->SetGenerateOverlapEvents(true);//开启生成堆叠事件
	}
	return SkeletalMeshComponent;
}


void ASceneBaseActor::SetItemOutline(bool bEnable)
{
	if (UStaticMeshComponent* SMC = GetStaticMeshComponent())
	{
		SMC->SetRenderCustomDepth(bEnable);
	}

	if (USkeletalMeshComponent* SKM = GetSkeletalMeshComponent())
	{
		SKM->SetRenderCustomDepth(bEnable);
	}
}

void ASceneBaseActor::SetItemPrompt(bool bEnable)
{
	if (!PromptWidget) return;;
	PromptWidget->SetVisibility(bEnable);
}


