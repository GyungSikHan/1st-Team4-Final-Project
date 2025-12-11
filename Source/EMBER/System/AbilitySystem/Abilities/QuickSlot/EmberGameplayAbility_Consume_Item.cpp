// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberGameplayAbility_Consume_Item.h"

#include "EmberPlayerCharacter.h"
#include "ItemInstance.h"
#include "StatusComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameInfo/GameplayTags.h"
#include "Managers/InventoryEquipmentManagerComponent.h"

UEmberGameplayAbility_Consume_Item::UEmberGameplayAbility_Consume_Item(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bServerRespectsRemoteAbilityCancellation = false;
	
	ActivationPolicy = EEmberAbilityActivationPolicy::Manual;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;

	AbilityTags.AddTag(EmberGameplayTags::Ability_Consume_Item);
	ActivationOwnedTags.AddTag(EmberGameplayTags::Status_Consume_Item);
	
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = EmberGameplayTags::GameplayEvent_Consume_Item;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UEmberGameplayAbility_Consume_Item::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (TriggerEventData == nullptr)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	CachedGameplayEventData = *TriggerEventData;

	if (CachedGameplayEventData.OptionalObject == nullptr)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	if (UAbilityTask_PlayMontageAndWait* ConsumeItemMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("MeleeAttack"), ConsumeItemMontage, DefaultAnimationRate, NAME_None, false, 1.f, 0.f, false))
	{
		ConsumeItemMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontageFinished);
		ConsumeItemMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::OnMontageFinished);
		ConsumeItemMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageFinished);
		ConsumeItemMontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontageFinished);
		ConsumeItemMontageTask->ReadyForActivation();
	}

	if (UAbilityTask_WaitGameplayEvent* ConsumeItemBeginEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EmberGameplayTags::GameplayEvent_Montage_Begin, nullptr, true, true))
	{
		ConsumeItemBeginEventTask->EventReceived.AddDynamic(this, &ThisClass::OnConsumeItemBegin);
		ConsumeItemBeginEventTask->ReadyForActivation();
	}
}

void UEmberGameplayAbility_Consume_Item::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	FGameplayTag InstigatorTag = CachedGameplayEventData.InstigatorTags.First();
	
	if (InstigatorTag == EmberGameplayTags::Ability_QuickSlot_Handle)
	{	
		int32 SelectedSlotIndex = CachedGameplayEventData.EventMagnitude;

		AEmberPlayerCharacter* EmberCharacter = GetEmberCharacterFromActorInfo();
		if (EmberCharacter == nullptr)
			return;

		UInventoryEquipmentManagerComponent* InventoryEquipmentManager = EmberCharacter->GetComponentByClass<UInventoryEquipmentManagerComponent>();
		if (InventoryEquipmentManager == nullptr)
			return;

		EEquipmentSlotType EquipmentSlotType = UEquipmentManagerComponent::ConvertToEquipmentSlotType(EWeaponHandType::Count, (EEquipState)SelectedSlotIndex);
		int32 ItemCount = InventoryEquipmentManager->GetItemCount(EquipmentSlotType);

		if (ItemCount <= 0)
			return;
		
		InventoryEquipmentManager->RemoveEquipment_Unsafe(EquipmentSlotType, 1);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEmberGameplayAbility_Consume_Item::OnConsumeItemBegin(FGameplayEventData Payload)
{
	AEmberPlayerCharacter* EmberCharacter = GetEmberCharacterFromActorInfo();
	if (EmberCharacter == nullptr)
		return;

	UStatusComponent* StatusComponent = EmberCharacter->GetStatusComponent();
	if (StatusComponent == nullptr)
		return;
	
	const UItemInstance* ItemInstance = Cast<UItemInstance>(CachedGameplayEventData.OptionalObject);
	if (ItemInstance == nullptr)
		return;
	
	UItemFragment_Equipable_Utility* FragmentUtility = ItemInstance->FindFragmentByClass<UItemFragment_Equipable_Utility>();
	if (FragmentUtility == nullptr)
		return;

	for (const FConsumableEffect& ConsumableEffect : FragmentUtility->ConsumableEffects)
	{
		if (ConsumableEffect.EffectTag == EmberGameplayTags::ItemAttribute_Consumable_Add_Health)
		{
			StatusComponent->AddHP(ConsumableEffect.Magnitude);
		}
	}
}

void UEmberGameplayAbility_Consume_Item::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
