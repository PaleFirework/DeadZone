// Fill out your copyright notice in the Description page of Project Settings.


#include "DzPlayerCameraManager.h"

#include "Camera/CameraComponent.h"
#include "DeadZone/Player/DzPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"

void ADzPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);
	if (ADzPlayerCharacter* Player = Cast<ADzPlayerCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		if (bLastCrouch != Player->bIsCrouched)
		{
			if (Player->bIsCrouched)
			{
				TargetSpringOffsetZ = 30;
				
			}
			else
			{
				TargetSpringOffsetZ = -30;
			}
		}
		//每次都要同步状态，记录蹲的情况
		bLastCrouch = Player->bIsCrouched;
		
		TargetSpringOffsetZ = FMath::FInterpTo(TargetSpringOffsetZ, 0, DeltaTime, 10);
		
		Player->GetSprintArmComponent()->SetRelativeLocation(FVector(0, 0, TargetSpringOffsetZ));
		
		//处理瞄准时相机的偏移
		float OffsetY = 0;
		float CameraOffsetX = 0;

		if (Player->IsIronsight())
		{
			OffsetY = 80;
			CameraOffsetX = 150;
		}
		else
		{
			OffsetY = 0;
			CameraOffsetX = 0;
		}
		
		Player->GetSprintArmComponent()->SocketOffset.Y = FMath::FInterpTo(Player->GetSprintArmComponent()->SocketOffset.Y, OffsetY, DeltaTime, 10);
		float X = FMath::FInterpTo(Player->GetCameraComponent()->GetRelativeLocation().X, CameraOffsetX, DeltaTime, 10);
		Player->GetCameraComponent()->SetRelativeLocation(FVector(X, 0, 0));
	}
	Super::UpdateCamera(DeltaTime);
}
