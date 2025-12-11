// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberGameplayAbility_Dash_Check.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EmberPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameInfo/GameplayTags.h"
#include "System/AbilitySystem/EmberAbilitySystemComponent.h"


UEmberGameplayAbility_Dash_Check::UEmberGameplayAbility_Dash_Check(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;

	AbilityTags.AddTag(EmberGameplayTags::Ability_Dash_Check);
}

void UEmberGameplayAbility_Dash_Check::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (IsLocallyControlled() == false)
		return;

	EEmberDirection Direction;
	FVector MovementVector;
	GetMovementDirection(Direction, MovementVector);
	
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
	
	if (CharacterMovement->IsFalling() || Direction == EEmberDirection::None || Direction == EEmberDirection::Forward)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}
	
	if (HasAuthority(&CurrentActivationInfo))
	{
		Server_RequestDash(Direction, MovementVector);
		SendDashActiveEvent(Direction, MovementVector);
	}
}

void UEmberGameplayAbility_Dash_Check::Server_RequestDash_Implementation(EEmberDirection Direction, FVector MovementVector)
{
	SendDashActiveEvent(Direction, MovementVector);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEmberGameplayAbility_Dash_Check::SendDashActiveEvent(EEmberDirection Direction, FVector MovementVector)
{
	FHitResult HitResult;
	HitResult.Item = (int32)Direction;
	HitResult.Normal = MovementVector;
			
	FGameplayEventData Payload;
	Payload.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(HitResult);
			
	SendGameplayEvent(EmberGameplayTags::Ability_Dash_Active, Payload);
}