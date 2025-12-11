// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberGameplayAbility_MeleeCombo.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitConfirmCancel.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Interaction/Tasks/EmberAbilityTask_WaitInputStart.h"

UEmberGameplayAbility_MeleeCombo::UEmberGameplayAbility_MeleeCombo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UEmberGameplayAbility_MeleeCombo::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bInputPressed = false;
	bInputReleased = false;

	if (UAbilityTask_WaitInputRelease* InputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true))
	{
		InputReleaseTask->OnRelease.AddDynamic(this, &ThisClass::OnInputReleased);
		InputReleaseTask->ReadyForActivation();
	}

	WaitInputContinue();
	WaitInputStop();
}

void UEmberGameplayAbility_MeleeCombo::HandleMontageEvent(FGameplayEventData Payload)
{
	if (HasAuthority(&CurrentActivationInfo) == false)
		return;
	
	bool bCanContinue = NextAbilityClass && (bInputPressed || bInputReleased == false);
	if (bCanContinue)
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo())
		{
			AbilitySystemComponent->TryActivateAbilityByClass(NextAbilityClass);
		}
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UEmberGameplayAbility_MeleeCombo::WaitInputContinue()
{
	if (UEmberAbilityTask_WaitInputStart* InputStartTask = UEmberAbilityTask_WaitInputStart::WaitInputStart(this))
	{
		InputStartTask->OnStart.AddDynamic(this, &ThisClass::OnInputStart);
		InputStartTask->ReadyForActivation();
	}
}

void UEmberGameplayAbility_MeleeCombo::WaitInputStop()
{
	if (UAbilityTask_WaitConfirmCancel* InputConfirmCancelTask = UAbilityTask_WaitConfirmCancel::WaitConfirmCancel(this))
	{
		InputConfirmCancelTask->OnCancel.AddDynamic(this, &ThisClass::OnInputCancel);
		InputConfirmCancelTask->ReadyForActivation();
	}
}

void UEmberGameplayAbility_MeleeCombo::OnInputReleased(float TimeHeld)
{
	bInputReleased = true;
}

void UEmberGameplayAbility_MeleeCombo::OnInputStart()
{
	bInputPressed = true;
	WaitInputContinue();
}

void UEmberGameplayAbility_MeleeCombo::OnInputCancel()
{
	bInputPressed = false;
	WaitInputStop();
}
