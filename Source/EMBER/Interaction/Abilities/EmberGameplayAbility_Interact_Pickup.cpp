// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberGameplayAbility_Interact_Pickup.h"

#include "EmberPlayerController.h"
#include "Interaction/Actors/Pickup/EmberInteractionActorBase.h"
#include "Managers/ItemManagerComponent.h"

UEmberGameplayAbility_Interact_Pickup::UEmberGameplayAbility_Interact_Pickup(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bServerRespectsRemoteAbilityCancellation = false;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnlyTermination;
}

void UEmberGameplayAbility_Interact_Pickup::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (TriggerEventData == nullptr || bInitialized == false)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	if (HasAuthority(&CurrentActivationInfo) == false)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;	
	}

	UItemManagerComponent* ItemManager = GetEmberPlayerControllerFromActorInfo()->GetComponentByClass<UItemManagerComponent>();
	if (ItemManager == nullptr)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	AEmberWorldPickupable* PickupableActor = Cast<AEmberWorldPickupable>(InteractableActor);
	if (PickupableActor == nullptr)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	if (ItemManager->TryPickupItem(PickupableActor))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}
