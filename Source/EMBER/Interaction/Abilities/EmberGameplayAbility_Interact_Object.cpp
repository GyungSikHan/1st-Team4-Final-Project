// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberGameplayAbility_Interact_Object.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameInfo/GameplayTags.h"
#include "Interaction/EmberInteractable.h"
#include "Interaction/Actors/EmberWorldInteractable.h"

UEmberGameplayAbility_Interact_Object::UEmberGameplayAbility_Interact_Object(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ActivationPolicy = EEmberAbilityActivationPolicy::Manual;

	FGameplayTagContainer DefaultAbilityTags;
	DefaultAbilityTags.AddTag(EmberGameplayTags::Ability_Interact_Object);
	SetAssetTags(DefaultAbilityTags);
	ActivationOwnedTags.AddTag(EmberGameplayTags::Status_Interact);
}

void UEmberGameplayAbility_Interact_Object::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (TriggerEventData == nullptr)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	AActor* TargetActor = const_cast<AActor*>(TriggerEventData->Target.Get());
	if (InitializeAbility(TargetActor) == false)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	bInitialized = false;

	if (Interactable->CanInteraction(GetAbilitySystemComponentFromActorInfo()) == false)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo) == false)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	bInitialized = true;
	
	if (AEmberWorldInteractable* WorldInteractable = Cast<AEmberWorldInteractable>(TargetActor))
	{
		WorldInteractable->OnInteractionSuccess(GetAvatarActorFromActorInfo());
	}
}

void UEmberGameplayAbility_Interact_Object::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
