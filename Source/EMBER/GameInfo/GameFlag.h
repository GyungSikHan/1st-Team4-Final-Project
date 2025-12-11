#pragma once
#include "CoreMinimal.h"

UENUM()
enum class ESpeedType : uint8
{
	Walk UMETA(DisplayName = "Walk"),
	Run UMETA(DisplayName = "Run"),
	Sprint UMETA(DisplayName = "Sprint"),
	Max UMETA(DisplayName = "Max")
};

UENUM()
enum class EFlySpeedType : uint8
{
	Fly_Stationary UMETA(DisplayName = "Fly_Stationary"),
	Fly_Move UMETA(DisplayName = "Fly_Movement"),
	Fly_Swoop UMETA(DisplayName = "Fly_Swoop"),
	Max UMETA(DisplayName = "Max")
};

UENUM()
enum class EStateType : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Run UMETA(DisplayName = "Run"),
	Anim UMETA(DisplayName = "Anim"),
	Detect UMETA(DisplayName = "Detect"),
	Equip UMETA(DisplayName = "Equip"),
	Action UMETA(DisplayName = "Action"),
	Combat UMETA(DisplayName = "Combat"),
	Hitted UMETA(DisplayName = "Hitted"),
	Dead UMETA(DisplayName = "Dead"),
	Max UMETA(DisplayName = "Max")
};

UENUM()
enum class EAICombatType : uint8
{
	Passive UMETA(DisplayName = "Passive"),
	Defensive UMETA(DisplayName = "Defensive"),
	Aggressive UMETA(DisplayName = "Aggressive"),
	Max UMETA(DisplayName = "Max")
};

UENUM()
enum class ESoundCategory : uint8
{
	Move UMETA(DisplayName = "Move"),
	Attack UMETA(DisplayName = "Attack"),
	Hit UMETA(DisplayName = "Hit"),
	Death UMETA(DisplayName = "Death"),
	Roar UMETA(DisplayName = "Roar"),
	Max UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class EEffectType : uint8
{
	Niagara,
	Particle
};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	Common,
	Uncommon,
	Rare,
	Unique,
	Legendary,

	Count	UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	Armor,
	Weapon,
	Utility,
	Crafting,
	
	Count	UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EUtilityType : uint8
{
	Consumable,
	LightSource,

	Count	UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ECraftingType : uint8
{
	Ore,
	Tree,
	Bone,
	Fur,
	Meat,
	Wood,
	Fiber,
	Fruit,
	String,

	Count	UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EWeaponHandType : uint8
{
	LeftHand,
	RightHand,
	TwoHand,

	Count	UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EEquipmentSlotType : uint8
{
	Unarmed_LeftHand,
	Unarmed_RightHand,
	
	Primary_LeftHand,
	Primary_RightHand,
	Primary_TwoHand,

	Utility_Primary,
	Utility_Secondary,
	Utility_Tertiary,
	Utility_Quaternary,
	
	Helmet,
	Shoulder,
	Cloak,
	Chest,
	Gloves,
	Pants,
	Foot,
	
	Count	UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Unarmed,
	OneHandSword,
	TwoHandSword,
	Shield,
	Tool,

	Count	UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EArmorType : uint8
{
	Helmet,
	Shoulder,
	Cloak,
	Chest,
	Gloves,
	Pants,
	Foot,

	Count	UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ESlotState : uint8
{
	Default,
	Invalid,
	Valid
};

UENUM(BlueprintType)
enum class EWeaponSlotType : uint8
{
	Primary,

	Count	UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EUtilitySlotType : uint8
{
	Primary,
	Secondary,
	Tertiary,
	Quaternary,

	Count	UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EEquipState : uint8
{
	Unarmed,
	
	Weapon_Primary,

	Utility_Primary,
	Utility_Secondary,
	Utility_Tertiary,
	Utility_Quaternary,

	Count	UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EEmberDirection : uint8
{
	None,
	Forward,
	Right,	
	Backward,
	Left,

	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class AISoundCategory : uint8
{
	MoveSound UMETA(DisplayName = "Move"),
	AttackSound UMETA(DisplayName = "Attack"),
	HitSound UMETA(DisplayName = "Hit"),
	DeathSound UMETA(DisplayName = "Death"),
	Max
};

UENUM(BlueprintType)
enum class EDeathState : uint8
{
	NotDead = 0,
	DeathStarted,
	DeathFinished
};

UENUM(BlueprintType)
enum class EBodyTemperatureChange : uint8
{
	Increase,
	Decrease,
	MAX UMETA(Hidden)
};