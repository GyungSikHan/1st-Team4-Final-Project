// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/AbilitySystem/Abilities/EmberGameplayAbility.h"
#include "EmberGameplayAbility_RewardItem.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UEmberGameplayAbility_RewardItem : public UEmberGameplayAbility
{
	GENERATED_BODY()

public:
	UEmberGameplayAbility_RewardItem(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnRewardItem(FGameplayEventData Payload);
};
