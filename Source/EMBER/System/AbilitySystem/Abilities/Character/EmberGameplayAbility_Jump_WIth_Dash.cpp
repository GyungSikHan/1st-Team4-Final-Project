// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberGameplayAbility_Jump_WIth_Dash.h"

#include "EmberPlayerCharacter.h"
#include "GameFlag.h"
#include "StatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameInfo/GameplayTags.h"
#include "System/AbilitySystem/EmberAbilitySystemComponent.h"

class UCharacterMovementComponent;

UEmberGameplayAbility_Jump_WIth_Dash::UEmberGameplayAbility_Jump_WIth_Dash(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	
	AbilityTags.AddTag(EmberGameplayTags::Ability_Jump);
	ActivationOwnedTags.AddTag(EmberGameplayTags::Status_Jump);
}

void UEmberGameplayAbility_Jump_WIth_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (IsLocallyControlled() == false)
		return;
	
	AEmberPlayerCharacter* EmberCharacter = GetEmberCharacterFromActorInfo();
	if (EmberCharacter == nullptr)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}
	
	UCharacterMovementComponent* CharacterMovement = EmberCharacter->GetCharacterMovement();
	if (CharacterMovement == nullptr)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	EEmberDirection Direction;
	FVector MovementVector;
	GetMovementDirection(Direction, MovementVector);
	
	if ((Direction != EEmberDirection::None && Direction != EEmberDirection::Forward) || CharacterMovement->IsFalling())
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	UEmberAbilitySystemComponent* AbilitySystemComponent = GetEmberAbilitySystemComponentFromActorInfo();
	if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(EmberGameplayTags::Status_Dash))
	{
 		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	Server_RequestJump();
	
	if (HasAuthority(&CurrentActivationInfo) == false)
	{
		StartJump();
	}
}

void UEmberGameplayAbility_Jump_WIth_Dash::Server_RequestJump_Implementation()
{
	StartJump();
}

void UEmberGameplayAbility_Jump_WIth_Dash::StartJump()
{
	CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	if (AEmberPlayerCharacter* EmberCharacter = GetEmberCharacterFromActorInfo())
	{
		if (EmberCharacter->IsLocallyControlled() && EmberCharacter->bPressedJump == false)
		{
			EmberCharacter->UnCrouch();
			UStatusComponent* status = EmberCharacter->GetStatusComponent();
			if (status != nullptr)
			{
				if (status->GetStamina() < EmberCharacter->GetJumpAmount())
					return;
				status->UseStamina(EmberCharacter->GetJumpAmount());
				EmberCharacter->Jump();
			}
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

