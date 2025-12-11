// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberGameplayAbility_Jump.h"

#include "EmberPlayerCharacter.h"
#include "StatusComponent.h"
#include "GameInfo/GameplayTags.h"

UEmberGameplayAbility_Jump::UEmberGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	
	AbilityTags.AddTag(EmberGameplayTags::Ability_Jump);
	ActivationOwnedTags.AddTag(EmberGameplayTags::Status_Jump);
}

void UEmberGameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (HasAuthority(&CurrentActivationInfo))
	{
		Server_RequestJump();
		return;
	}

	if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo) == false)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}
	
	StartJump();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEmberGameplayAbility_Jump::Server_RequestJump_Implementation()
{
	CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	StartJump();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEmberGameplayAbility_Jump::StartJump()
{
	if (AEmberPlayerCharacter* EmberCharacter = GetEmberCharacterFromActorInfo())
	{
		if (EmberCharacter->IsLocallyControlled() && EmberCharacter->bPressedJump == false)
		{
			EmberCharacter->UnCrouch();

			if (EmberCharacter->HasAuthority())
			{
				UStatusComponent* status = EmberCharacter->GetStatusComponent();
				if (status != nullptr)
				{
					status->UseStamina(EmberCharacter->GetJumpAmount());
					GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT("%f"), status->GetStamina()));
				}
			}
			EmberCharacter->Jump();
		}
	}
}