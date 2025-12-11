// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmberGameplayAbility_Interact_Info.h"
#include "EmberGameplayAbility_Interact_Object.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UEmberGameplayAbility_Interact_Object : public UEmberGameplayAbility_Interact_Info
{
	GENERATED_BODY()

public:
	UEmberGameplayAbility_Interact_Object(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~GameplayAbility Override
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End of GameplayAbility Override

protected:
	bool bInitialized = false;
};
