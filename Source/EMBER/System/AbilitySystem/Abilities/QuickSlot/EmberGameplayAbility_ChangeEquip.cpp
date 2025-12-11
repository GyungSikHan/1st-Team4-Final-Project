// Fill out your copyright notice in the Description page of Project Settings.

#include "EmberGameplayAbility_ChangeEquip.h"

#include "EmberPlayerCharacter.h"
#include "GameInfo/GameplayTags.h"
#include "Managers/EquipmentManagerComponent.h"

UEmberGameplayAbility_ChangeEquip::UEmberGameplayAbility_ChangeEquip(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ActivationPolicy = EEmberAbilityActivationPolicy::Manual;
	bServerRespectsRemoteAbilityCancellation = false;

	AbilityTags.AddTag(EmberGameplayTags::Ability_ChangeEquip);
	ActivationOwnedTags.AddTag(EmberGameplayTags::Status_ChangeEquip);

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = EmberGameplayTags::GameplayEvent_ChangeEquip;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UEmberGameplayAbility_ChangeEquip::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (TriggerEventData == nullptr)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	AEmberPlayerCharacter* EmberCharacter = GetEmberCharacterFromActorInfo();
	if (EmberCharacter == nullptr)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	UEquipmentManagerComponent* EquipmentManager = EmberCharacter->GetComponentByClass<UEquipmentManagerComponent>();
	if (EquipmentManager == nullptr)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	int32 NewEquipStateIndex = FMath::RoundToInt(TriggerEventData->EventMagnitude);
	EEquipState NewEquipState = (EEquipState)NewEquipStateIndex;
	
	if (EquipmentManager->CanChangeEquipState(NewEquipState) == false)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}
	
	if (HasAuthority(&CurrentActivationInfo))
	{
		EquipmentManager->ChangeEquipState(NewEquipState);
	}
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}, EndDelay, false);
}
