// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberGameplayAbility_Dash_Active.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EmberPlayerController.h"
#include "GameFlag.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/RootMotionSource.h"
#include "GameInfo/GameplayTags.h"

UEmberGameplayAbility_Dash_Active::UEmberGameplayAbility_Dash_Active(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ActivationPolicy = EEmberAbilityActivationPolicy::Manual;
	
	AbilityTags.AddTag(EmberGameplayTags::Ability_Dash_Active);
	ActivationOwnedTags.AddTag(EmberGameplayTags::Status_Dash);

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = EmberGameplayTags::Ability_Dash_Active;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UEmberGameplayAbility_Dash_Active::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (TriggerEventData == nullptr)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo) == false)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	if (AEmberPlayerController* EmberPlayerController = GetEmberPlayerControllerFromActorInfo())
	{
		EmberPlayerController->SetIgnoreMoveInput(true);
	}

	const FHitResult& HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TriggerEventData->TargetData, 0);
	EEmberDirection Direction = (EEmberDirection)HitResult.Item;

	UAnimMontage* SelectedMontage = SelectDirectionalMontage(Direction);
	if (UAbilityTask_PlayMontageAndWait* DashMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("DashMontage"), SelectedMontage, 1.f, NAME_None, true))
	{
		DashMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageFinished);
		DashMontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnMontageFinished);
		DashMontageTask->ReadyForActivation();
	}

	FVector MovementVector = HitResult.Normal;
	if (UAbilityTask_ApplyRootMotionConstantForce* RootMotionForceTask = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
		this, TEXT("DashForce"), MovementVector, DashStrength, DashDuration, true, nullptr, ERootMotionFinishVelocityMode::ClampVelocity, FVector::ZeroVector, 100.f, true))
	{
		RootMotionForceTask->OnFinish.AddDynamic(this, &ThisClass::OnDashFinished);
		RootMotionForceTask->ReadyForActivation();
	}
}

void UEmberGameplayAbility_Dash_Active::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (AEmberPlayerController* EmberPlayerController = GetEmberPlayerControllerFromActorInfo())
	{
		EmberPlayerController->SetIgnoreMoveInput(false);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

UAnimMontage* UEmberGameplayAbility_Dash_Active::SelectDirectionalMontage(EEmberDirection MovementDirection) const
{
	UAnimMontage* AnimMontage = nullptr;
	switch (MovementDirection)
	{
	case EEmberDirection::Right:		AnimMontage = RightMontage;		break;
	case EEmberDirection::Backward:	AnimMontage = BackwardMontage;	break;
	case EEmberDirection::Left:		AnimMontage = LeftMontage;		break;
	}
	return AnimMontage;
}

void UEmberGameplayAbility_Dash_Active::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

void UEmberGameplayAbility_Dash_Active::OnDashFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

