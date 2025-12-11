// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/AbilitySystem/Abilities/EmberGameplayAbility.h"
#include "EmberGameplayAbility_ChangeEquip.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UEmberGameplayAbility_ChangeEquip : public UEmberGameplayAbility
{
	GENERATED_BODY()

public:
	UEmberGameplayAbility_ChangeEquip(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Change Equip")
	float EndDelay = 0.6f;
};
