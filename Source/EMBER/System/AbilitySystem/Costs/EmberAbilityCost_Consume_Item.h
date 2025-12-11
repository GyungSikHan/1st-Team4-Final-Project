// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmberAbilityCost.h"
#include "GameFlag.h"
#include "EmberAbilityCost_Consume_Item.generated.h"


UCLASS(meta=(DisplayName="Equipped Item"))
class EMBER_API UEmberAbilityCost_Consume_Item : public UEmberAbilityCost
{
	GENERATED_BODY()

public:
	UEmberAbilityCost_Consume_Item();

protected:
	virtual bool CheckCost(const UEmberGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const UEmberGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AbilityCost")
	FScalableFloat Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AbilityCost")
	EEquipmentType EquipmentType = EEquipmentType::Count;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AbilityCost", meta=(EditCondition="EquipmentType == EEquipmentType::Weapon", EditConditionHides))
	EWeaponHandType WeaponHandType = EWeaponHandType::Count;
};
