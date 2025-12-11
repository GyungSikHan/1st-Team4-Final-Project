// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmberGameplayAbility_Interact_Object.h"
#include "EmberGameplayAbility_Interact_Pickup.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UEmberGameplayAbility_Interact_Pickup : public UEmberGameplayAbility_Interact_Object
{
	GENERATED_BODY()

public:
	UEmberGameplayAbility_Interact_Pickup(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
