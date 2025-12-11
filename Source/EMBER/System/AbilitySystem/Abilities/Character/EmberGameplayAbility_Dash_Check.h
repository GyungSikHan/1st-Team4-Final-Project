// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/AbilitySystem/Abilities/EmberGameplayAbility.h"
#include "EmberGameplayAbility_Dash_Check.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UEmberGameplayAbility_Dash_Check : public UEmberGameplayAbility
{
	GENERATED_BODY()

public:
	UEmberGameplayAbility_Dash_Check(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION(Server, Reliable)
	void Server_RequestDash(EEmberDirection Direction, FVector MovementVector);

private:
	void SendDashActiveEvent(EEmberDirection Direction, FVector MovementVector);
};
