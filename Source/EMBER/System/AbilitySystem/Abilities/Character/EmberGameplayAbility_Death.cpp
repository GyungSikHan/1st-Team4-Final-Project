// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberGameplayAbility_Death.h"

#include "EmberPlayerCharacter.h"
#include "StatusComponent.h"
#include "Abilities/Tasks/AbilityTask_NetworkSyncPoint.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameInfo/GameplayTags.h"

UEmberGameplayAbility_Death::UEmberGameplayAbility_Death(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ActivationPolicy = EEmberAbilityActivationPolicy::Manual;
	
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;

	AbilityTags.AddTag(EmberGameplayTags::Ability_Death);

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = EmberGameplayTags::GameplayEvent_Death;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UEmberGameplayAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	SetCanBeCanceled(false);

	if (UAbilityTask_NetworkSyncPoint* NetSyncTask = UAbilityTask_NetworkSyncPoint::WaitNetSync(this, EAbilityTaskNetSyncType::OnlyServerWait))
	{
		NetSyncTask->OnSync.AddDynamic(this, &ThisClass::OnNetSync);
		NetSyncTask->ReadyForActivation();
	}

	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}, EndDelay, false);
}

void UEmberGameplayAbility_Death::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	FinishDeath();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEmberGameplayAbility_Death::OnNetSync()
{
	AEmberPlayerCharacter* TargetCharacter = GetEmberCharacterFromActorInfo();
	if (TargetCharacter == nullptr)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	if (UAbilityTask_PlayMontageAndWait* HitReactMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("DeathMontage"), DeathMontage, 1.f, NAME_None, true))
	{
		HitReactMontageTask->ReadyForActivation();
	}
}

void UEmberGameplayAbility_Death::StartDeath()
{
	if (AEmberPlayerCharacter* EmberCharacter = GetEmberCharacterFromActorInfo())
	{
		if (UStatusComponent* StatusComponent = EmberCharacter->FindComponentByClass<UStatusComponent>())
		{
			if (StatusComponent->GetDeathState() == EDeathState::NotDead)
			{
				StatusComponent->StartDeath();
			}
		}
	}
}

void UEmberGameplayAbility_Death::FinishDeath()
{
	if (AEmberPlayerCharacter* EmberCharacter = GetEmberCharacterFromActorInfo())
	{
		if (UStatusComponent* StatusComponent = EmberCharacter->FindComponentByClass<UStatusComponent>())
		{
			if (StatusComponent->GetDeathState() == EDeathState::DeathStarted)
			{
				StatusComponent->FinishDeath();
			}
		}
	}
}
