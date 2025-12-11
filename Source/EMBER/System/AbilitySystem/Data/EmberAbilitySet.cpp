// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberAbilitySet.h"

#include "System/AbilitySystem/EmberAbilitySystemComponent.h"
#include "System/AbilitySystem/Abilities/EmberGameplayAbility.h"

void FEmberAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FEmberAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FEmberAbilitySet_GrantedHandles::TakeFromAbilitySystem(UEmberAbilitySystemComponent* EmberASC)
{
	if (!EmberASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			EmberASC->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			EmberASC->RemoveActiveGameplayEffect(Handle);
		}
	}

	GrantedAttributeSets.Reset();
	GameplayEffectHandles.Reset();
}

UEmberAbilitySet::UEmberAbilitySet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UEmberAbilitySet::GiveToAbilitySystem(UEmberAbilitySystemComponent* EmberASC, FEmberAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(EmberASC);

	if (EmberASC->IsOwnerActorAuthoritative() == false)
	{
		return;
	}
	
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FEmberAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant.Ability))
		{
			continue;
		}

		UEmberGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UEmberGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = EmberASC->GiveAbility(AbilitySpec);
		
		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
	{
		const FEmberAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			continue;
		}

		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle GameplayEffectHandle = EmberASC->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, EmberASC->MakeEffectContext());

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}
}
