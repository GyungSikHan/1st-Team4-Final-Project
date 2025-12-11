// Fill out your copyright notice in the Description page of Project Settings.

#include "EmberAbilityTask_WaitInputStart.h"

#include "AbilitySystemComponent.h"

UEmberAbilityTask_WaitInputStart::UEmberAbilityTask_WaitInputStart(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

UEmberAbilityTask_WaitInputStart* UEmberAbilityTask_WaitInputStart::WaitInputStart(UGameplayAbility* OwningAbility)
{
	UEmberAbilityTask_WaitInputStart* Task = NewAbilityTask<UEmberAbilityTask_WaitInputStart>(OwningAbility);
	return Task;
}

void UEmberAbilityTask_WaitInputStart::Activate()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC && Ability)
	{
		DelegateHandle = ASC->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::GameCustom1, GetAbilitySpecHandle(), GetActivationPredictionKey()).AddUObject(this, &UEmberAbilityTask_WaitInputStart::OnStartCallback);
		if (IsForRemoteClient())
		{
			if (ASC->CallReplicatedEventDelegateIfSet(EAbilityGenericReplicatedEvent::GameCustom1, GetAbilitySpecHandle(), GetActivationPredictionKey()) == false)
			{
				SetWaitingOnRemotePlayerData();
			}
		}
	}
}

void UEmberAbilityTask_WaitInputStart::OnStartCallback()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (Ability == nullptr || ASC == nullptr)
		return;

	ASC->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::GameCustom1, GetAbilitySpecHandle(), GetActivationPredictionKey()).Remove(DelegateHandle);

	FScopedPredictionWindow ScopedPrediction(ASC, IsPredictingClient());

	if (IsPredictingClient())
	{
		ASC->ServerSetReplicatedEvent(EAbilityGenericReplicatedEvent::GameCustom1, GetAbilitySpecHandle(), GetActivationPredictionKey(), ASC->ScopedPredictionKey);
	}
	else
	{
		ASC->ConsumeGenericReplicatedEvent(EAbilityGenericReplicatedEvent::GameCustom1, GetAbilitySpecHandle(), GetActivationPredictionKey());
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnStart.Broadcast();	
	}
	
	EndTask();
}
