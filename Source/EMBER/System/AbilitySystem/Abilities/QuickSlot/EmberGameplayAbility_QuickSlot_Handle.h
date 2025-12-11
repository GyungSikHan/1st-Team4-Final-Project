// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/AbilitySystem/Abilities/EmberGameplayAbility.h"
#include "EmberGameplayAbility_QuickSlot_Handle.generated.h"

class UItemInstance;

UCLASS()
class EMBER_API UEmberGameplayAbility_QuickSlot_Handle : public UEmberGameplayAbility
{
	GENERATED_BODY()

public:
	UEmberGameplayAbility_QuickSlot_Handle(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	bool IsConsumableItem(UItemInstance* ItemInstance);

private:
	UPROPERTY(EditDefaultsOnly)
	float EndDelay = 0.6f;
};
