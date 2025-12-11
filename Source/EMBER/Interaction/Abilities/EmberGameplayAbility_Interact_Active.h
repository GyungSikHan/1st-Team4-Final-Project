// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EmberGameplayAbility_Interact_Info.h"
#include "EmberGameplayAbility_Interact_Active.generated.h"

UCLASS()
class EMBER_API UEmberGameplayAbility_Interact_Active : public UEmberGameplayAbility_Interact_Info
{
	GENERATED_BODY()

public:
	UEmberGameplayAbility_Interact_Active(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	bool TriggerInteraction();

};

