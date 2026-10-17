#pragma once

#define CUSTOM_KEY_SAVE_NAME TEXT("CustomKey")

#define WEAPON_INDEX 100
#define CONSUMABLE_INDEX 1000


#define WEAPON_TRACE ECC_GameTraceChannel2

UENUM(BlueprintType)
enum class ETeamType : uint8
{
	ETT_Player,
	ETT_Enemy,
	ETT_Zombie,
};