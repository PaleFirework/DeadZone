// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ReloadWeapon.h"

#include "DeadZone/Character/DzCharacterBase.h"
#include "DeadZone/Weapon/WeaponActor.h"

void UAnimNotify_ReloadWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      const FAnimNotifyEventReference& EventReference)
{
	if (ADzCharacterBase* CharacterBase = Cast<ADzCharacterBase>(MeshComp->GetOwner()))
	{
		if (CharacterBase->GetHoldWeapon())
		{
			CharacterBase->GetHoldWeapon()->MakeFullClip();
		}
	}
}
