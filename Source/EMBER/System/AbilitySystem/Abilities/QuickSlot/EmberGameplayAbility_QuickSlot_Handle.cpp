// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberGameplayAbility_QuickSlot_Handle.h"

#include "EmberPlayerCharacter.h"
#include "ItemInstance.h"
#include "Fragments/ItemFragment_Equipable_Utility.h"
#include "GameInfo/GameplayTags.h"
#include "Managers/EquipmentManagerComponent.h"
#include "Managers/InventoryEquipmentManagerComponent.h"

class UEquipmentManagerComponent;

UEmberGameplayAbility_QuickSlot_Handle::UEmberGameplayAbility_QuickSlot_Handle(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bServerRespectsRemoteAbilityCancellation = false;
	
	ActivationPolicy = EEmberAbilityActivationPolicy::Manual;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
	
	AbilityTags.AddTag(EmberGameplayTags::Ability_QuickSlot_Handle);
	ActivationOwnedTags.AddTag(EmberGameplayTags::Status_QuickSlot_Handle);

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = EmberGameplayTags::GameplayEvent_QuickSlot_Handle;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UEmberGameplayAbility_QuickSlot_Handle::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (TriggerEventData == nullptr)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	int32 NewEquipStateIndex = FMath::RoundToInt(TriggerEventData->EventMagnitude);
	EEquipState NewEquipState = (EEquipState)NewEquipStateIndex;

	AEmberPlayerCharacter* EmberCharacter = GetEmberCharacterFromActorInfo();
	if (EmberCharacter == nullptr)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}
	
	UInventoryEquipmentManagerComponent* InventoryEquipmentManager = EmberCharacter->GetComponentByClass<UInventoryEquipmentManagerComponent>();
	if (InventoryEquipmentManager == nullptr)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	EEquipmentSlotType EquipmentSlotType = UEquipmentManagerComponent::ConvertToEquipmentSlotType(EWeaponHandType::Count, NewEquipState);
	UItemInstance* ItemInstance = InventoryEquipmentManager->GetItemInstance(EquipmentSlotType);
	if (IsConsumableItem(ItemInstance))
	{
		FGameplayEventData Payload;
		Payload.OptionalObject = ItemInstance;
		Payload.EventMagnitude = NewEquipStateIndex;
		Payload.InstigatorTags.AddTag(EmberGameplayTags::Ability_QuickSlot_Handle);
		SendGameplayEvent(EmberGameplayTags::GameplayEvent_Consume_Item, Payload);
	}
	else
	{
		FGameplayEventData Payload;
		Payload.EventMagnitude = NewEquipStateIndex;
		SendGameplayEvent(EmberGameplayTags::GameplayEvent_ChangeEquip, Payload);
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}, EndDelay, false);
}

bool UEmberGameplayAbility_QuickSlot_Handle::IsConsumableItem(UItemInstance* ItemInstance)
{
	if (ItemInstance == nullptr)
	{
		return false;
	}
	
	const UItemFragment_Equipable_Utility* FragmentUtility = ItemInstance->FindFragmentByClass<UItemFragment_Equipable_Utility>();
	if (FragmentUtility == nullptr)
	{
		return false;
	}

	return FragmentUtility->UtilityType == EUtilityType::Consumable;
}
