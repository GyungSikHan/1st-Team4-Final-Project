#include "GameplayTags.h"

namespace EmberGameplayTags
{
	// Message
	UE_DEFINE_GAMEPLAY_TAG(Message_Initialize_MyInventory,					"Message.Initialize.Inventory");
	UE_DEFINE_GAMEPLAY_TAG(Message_Initialize_OtherInventory,				"Message.Initialize.OtherInventory");
	UE_DEFINE_GAMEPLAY_TAG(Message_Initialize_InventoryEquipment,			"Message.Initialize.InventoryEquipment");
	UE_DEFINE_GAMEPLAY_TAG(Message_Initialize_CraftingWidget,				"Message.Initialize.CraftingWidget");
	UE_DEFINE_GAMEPLAY_TAG(Message_Interaction_Notice,						"Message.Interaction.Notice");
	UE_DEFINE_GAMEPLAY_TAG(Message_Interaction_Progress,					"Message.Interaction.Progress");
	UE_DEFINE_GAMEPLAY_TAG(Message_ActionHint_Notice,						"Message.ActionHint.Notice");

	// Input
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Attack_MainHand,						"InputTag.Attack.MainHand");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Interact,								"InputTag.Interact");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Widget_Inventory,						"InputTag.Widget.Inventory");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Widget_CheatManager,					"InputTag.Widget.CheatManager");
	
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Movement_Move,							"InputTag.Movement.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Movement_Look,							"InputTag.Movement.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Movement_Sprint,						"InputTag.Movement.Sprint");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Movement_Jump,							"InputTag.Movement.Jump");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Movement_Dash,							"InputTag.Movement.Dash");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_QuickSlot_Weapon_Primary,				"InputTag.QuickSlot.Weapon.Primary");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_QuickSlot_Utility_Primary,				"InputTag.QuickSlot.Utility.Primary");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_QuickSlot_Utility_Secondary,			"InputTag.QuickSlot.Utility.Secondary");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_QuickSlot_Utility_Tertiary,				"InputTag.QuickSlot.Utility.Tertiary");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_QuickSlot_Utility_Quaternary,			"InputTag.QuickSlot.Utility.Quaternary");

	// Ability
	UE_DEFINE_GAMEPLAY_TAG(Ability_Interact,								"Ability.Interact");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Interact_Object,							"Ability.Interact.Object");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Attack,									"Ability.Attack");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Attack_1,								"Ability.Attack.1");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Attack_2,								"Ability.Attack.2");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Attack_3,								"Ability.Attack.3");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Widget_Inventory,						"Ability.Widget.Inventory");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Widget_CheatManager,						"Ability.Widget.CheatManager"); /* Temporary */
	UE_DEFINE_GAMEPLAY_TAG(Ability_Interact_Active,							"Ability.Interact.Active");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Logging,									"Ability.Logging");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Mining,									"Ability.Mining");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Death,									"Ability_Death");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Jump,									"Ability.Jump");
	UE_DEFINE_GAMEPLAY_TAG(Ability_HitReact,								"Ability.HitReact");

	UE_DEFINE_GAMEPLAY_TAG(Ability_Dash_Check,								"Ability.Dash.Check");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Dash_Active,								"Ability.Dash.Active");

	UE_DEFINE_GAMEPLAY_TAG(Ability_QuickSlot_Handle,						"Ability.QuickSlot.Handle");
	UE_DEFINE_GAMEPLAY_TAG(Ability_ChangeEquip,								"Ability.ChangeEquip");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Consume_Item,							"Ability.Consume_item");
	
	UE_DEFINE_GAMEPLAY_TAG(Ability_BodyTemperature,							"Ability.BodyTemperature");
	
	// ItemAttribute
	UE_DEFINE_GAMEPLAY_TAG(ItemAttribute_BaseDamage,						"ItemAttribute.BaseDamage");
	UE_DEFINE_GAMEPLAY_TAG(ItemAttribute_Defense,							"ItemAttribute.Defense");
	UE_DEFINE_GAMEPLAY_TAG(ItemAttribute_Consumable_Add_Health,				"ItemAttribute.Consumable.Add.Health");

	// Status
	UE_DEFINE_GAMEPLAY_TAG(Status_Interact,									"Status.Interact");
	UE_DEFINE_GAMEPLAY_TAG(Status_ActionHint,								"Status.ActionHint");
	UE_DEFINE_GAMEPLAY_TAG(Status_Logging,									"Status.Logging");
	UE_DEFINE_GAMEPLAY_TAG(Status_Mining,									"Status.Mining");
	
	UE_DEFINE_GAMEPLAY_TAG(Status_Attack,									"Status.Attack");
	UE_DEFINE_GAMEPLAY_TAG(Status_HitReact,									"Status.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Status_Dash,										"Status.Dash");
	UE_DEFINE_GAMEPLAY_TAG(Status_Jump,										"Status.Jump");
	UE_DEFINE_GAMEPLAY_TAG(Status_Death,									"Status.Death");
	UE_DEFINE_GAMEPLAY_TAG(Status_Death_Dying,								"Status.Death_Dying");
	
	UE_DEFINE_GAMEPLAY_TAG(Status_QuickSlot_Handle,							"Status.QuickSlot.Handle");
	UE_DEFINE_GAMEPLAY_TAG(Status_ChangeEquip,								"Status.ChangeEquip");
	UE_DEFINE_GAMEPLAY_TAG(Status_Consume_Item,								"Status.Consume.Item");
	
	UE_DEFINE_GAMEPLAY_TAG(Status_BodyTemperature,							"Status.BodyTemperature");

	// Gameplay Event
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Montage_Begin,						"GameplayEvent.Montage.Begin");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Montage_Tick,						"GameplayEvent.Montage.Tick");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Montage_End,						"GameplayEvent.Montage.End");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Trace,								"GameplayEvent.Trace");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Reward_Item,						"GameplayEvent.Reward.Item");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_QuickSlot_Handle,					"GameplayEvent.QuickSlot_Handle");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_ChangeEquip,						"GameplayEvent.ChangeEquip");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Consume_Item,						"GameplayEvent.Consume.Item");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_HitReact,							"GameplayEvent.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Jump,								"GameplayEvent.Jump");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Death,								"GameplayEvent.Death");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_BodyTemperature_Change,			"GameplayEvent_BodyTemperature_Change");

	// Cooldown
	UE_DEFINE_GAMEPLAY_TAG(Cooldown_Dash,									"Cooldown.Dash");
}