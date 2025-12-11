// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmberGameplayAbility_Jump.h"
#include "EmberGameplayAbility_Jump_WIth_Dash.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UEmberGameplayAbility_Jump_WIth_Dash : public UEmberGameplayAbility
{
	GENERATED_BODY()

public:
	UEmberGameplayAbility_Jump_WIth_Dash(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION(Server, Reliable)
	void Server_RequestJump();
	
protected:
	UFUNCTION()
	void StartJump();
};
