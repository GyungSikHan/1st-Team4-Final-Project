// Fill out your copyright notice in the Description page of Project Settings.

#include "EmberGameplayAbility_ActionHint.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameInfo/GameplayTags.h"

UEmberGameplayAbility_ActionHint::UEmberGameplayAbility_ActionHint(const FObjectInitializer& ObjectInitializer)
{
	ActivationPolicy = EEmberAbilityActivationPolicy::OnSpawn;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
}

void UEmberGameplayAbility_ActionHint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		for (const FEmberActionHintInfo& ActionHintInfo : ActionHintInfos)
		{
			FOnGameplayEffectTagCountChanged& OnTagEvent = ASC->RegisterGameplayTagEvent(ActionHintInfo.AbilityTag, EGameplayTagEventType::NewOrRemoved);
			FDelegateHandle EventHandle = OnTagEvent.AddUObject(this, &ThisClass::OnTagChanged);
			EventHandleMap.Emplace(ActionHintInfo.AbilityTag, EventHandle);
		}
	}
}

void UEmberGameplayAbility_ActionHint::OnTagChanged(const FGameplayTag InAbilityTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		SetActionHintInfo(InAbilityTag);
	}
	else
	{
		if (CurrentActionHintInfo.AbilityTag == InAbilityTag)
		{
			ClearActionHintInfo();
		}
	}

	UpdateActionHint();
}

void UEmberGameplayAbility_ActionHint::SetActionHintInfo(FGameplayTag InAbilityTag)
{
	if (FEmberActionHintInfo* ActionHintInfo = ActionHintInfos.FindByKey(InAbilityTag))
	{
		CurrentActionHintInfo = *ActionHintInfo;
	}
}

void UEmberGameplayAbility_ActionHint::ClearActionHintInfo()
{
	CurrentActionHintInfo.AbilityTag = FGameplayTag::EmptyTag;
	CurrentActionHintInfo.Content = FText::GetEmpty();
	CurrentActionHintInfo.Title = FText::GetEmpty();
}

void UEmberGameplayAbility_ActionHint::UpdateActionHint()
{
	FEmberActionHintMessage Message;
	Message.bShouldRefresh = true;
	Message.ActionHintInfo = CurrentActionHintInfo;		
	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetAvatarActorFromActorInfo());
	MessageSystem.BroadcastMessage(EmberGameplayTags::Message_ActionHint_Notice, Message);
}

void UEmberGameplayAbility_ActionHint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		for (auto Pair : EventHandleMap)
		{
			FGameplayTag& AbilityTag = Pair.Key;
			FDelegateHandle& DelegateHandle = Pair.Value;
			ASC->UnregisterGameplayTagEvent(DelegateHandle, AbilityTag);
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}