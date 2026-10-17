// Fill out your copyright notice in the Description page of Project Settings.


#include "DzCharacterMovementComponent.h"

#include "DeadZone/Character/DzCharacterBase.h"


class ADzCharacterBase;
// Sets default values for this component's properties
UDzCharacterMovementComponent::UDzCharacterMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	MaxWalkSpeed = 470.f;
	MaxWalkSpeedCrouched = 200.f;
	MaxStepHeight = 20.f;
	
	NavAgentProps.bCanCrouch = true;
	
}


// Called when the game starts
void UDzCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

float UDzCharacterMovementComponent::GetMaxSpeed() const
{
	float Speed = Super::GetMaxSpeed();
	switch(MovementMode)
	{
	case MOVE_Walking:
	case MOVE_NavWalking:
		if (ADzCharacterBase* CharacterBase = Cast<ADzCharacterBase>(GetOwner()))
		{
			if (CharacterBase->IsSprinting())
			{
				Speed = SprintSpeed;
			} else if (CharacterBase->IsIronsight())
			{
				Speed *= 0.6;
			}
			
		}
		break;
	}
	return Speed;
}


// Called every frame
void UDzCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

