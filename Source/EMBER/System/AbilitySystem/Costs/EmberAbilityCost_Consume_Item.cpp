// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberAbilityCost_Consume_Item.h"

#include "EmberPlayerCharacter.h"
#include "GameInfo/GameplayTags.h"
#include "Managers/EquipmentManagerComponent.h"

UEmberAbilityCost_Consume_Item::UEmberAbilityCost_Consume_Item()
{
	Quantity.SetValue(1.f);
}

bool UEmberAbilityCost_Consume_Item::CheckCost(const UEmberGameplayAbility* Ability,
	const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if (EquipmentType == EEquipmentType::Count)
		return false;

	AEmberPlayerCharacter* EmberCharacter = Ability->GetEmberCharacterFromActorInfo();
	if (EmberCharacter == nullptr)
		return false;

	return false;
}

void UEmberAbilityCost_Consume_Item::ApplyCost(const UEmberGameplayAbility* Ability,
                                               const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                               const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::ApplyCost(Ability, Handle, ActorInfo, ActivationInfo);
}
