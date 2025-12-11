// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayAbilitySpecHandle.h"
#include "Interaction/Abilities/EmberGameplayAbility_Interact_Info.h"
#include "UObject/Object.h"
#include "EmberAbilityCost.generated.h"

class UEmberGameplayAbility;

UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class EMBER_API UEmberAbilityCost : public UObject
{
	GENERATED_BODY()

public:
	UEmberAbilityCost() {}

	virtual bool CheckCost(const UEmberGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
	{
		return false;
	}

	virtual void ApplyCost(const UEmberGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
	{
	}

	bool ShouldOnlyApplyCostOnHit() const { return bOnlyApplyCostOnHit; }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Costs)
	bool bOnlyApplyCostOnHit = false;
};
