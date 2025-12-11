// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberGameplayAbility_BodyTemperature.h"

#include "GameFlag.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameInfo/GameplayTags.h"
#include "Tasks/EmberAbilityTask_WeatherCondition.h"

UEmberGameplayAbility_BodyTemperature::UEmberGameplayAbility_BodyTemperature(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnly;
	
	ActivationPolicy = EEmberAbilityActivationPolicy::OnSpawn;

	FGameplayTagContainer DefaultAbilityTags;
	DefaultAbilityTags.AddTag(EmberGameplayTags::Ability_BodyTemperature);
	SetAssetTags(DefaultAbilityTags);
	ActivationOwnedTags.AddTag(EmberGameplayTags::Status_BodyTemperature);
	
	DecreaseIntervalRate = 1.0f;
	DecreaseAmount = 0.1f;
}

void UEmberGameplayAbility_BodyTemperature::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	WeatherCondition = UEmberAbilityTask_WeatherCondition::WaitForDecreaseBodyTemperature(this, FEmberBodyTemperatureQuery(), DecreaseIntervalRate, DecreaseAmount);
	if (WeatherCondition)
	{
		WeatherCondition->ReadyForActivation();
	}
	
	if (UAbilityTask_WaitGameplayEvent* GameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EmberGameplayTags::GameplayEvent_BodyTemperature_Change, nullptr, false, true))
	{
		GameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::OnChangeBodyTemperature);
		GameplayEventTask->ReadyForActivation();
	}
}

void UEmberGameplayAbility_BodyTemperature::OnChangeBodyTemperature(FGameplayEventData TriggerEventData)
{
	EBodyTemperatureChange BodyTemperature = (EBodyTemperatureChange)TriggerEventData.EventMagnitude;
	if (BodyTemperature == EBodyTemperatureChange::Increase)
	{
		if (WeatherCondition && WeatherCondition->IsActive())
		{
			WeatherCondition->EndTask();
		}
	}
	else if (BodyTemperature == EBodyTemperatureChange::Decrease)
	{
		if (WeatherCondition && WeatherCondition->IsActive())
		{
			WeatherCondition->EndTask();
		}
		
		WeatherCondition = UEmberAbilityTask_WeatherCondition::WaitForDecreaseBodyTemperature(this, FEmberBodyTemperatureQuery(), DecreaseIntervalRate, DecreaseAmount);
		if (WeatherCondition)
		{
			WeatherCondition->ReadyForActivation();
		}
	}
}

