// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/AbilitySystem/Abilities/EmberGameplayAbility.h"
#include "EmberGameplayAbility_Consume_Item.generated.h"

class UItemInstance;

UCLASS()
class EMBER_API UEmberGameplayAbility_Consume_Item : public UEmberGameplayAbility
{
	GENERATED_BODY()

public:
	UEmberGameplayAbility_Consume_Item(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UFUNCTION()
	void OnConsumeItemBegin(FGameplayEventData Payload);
	
	UFUNCTION()
	void OnMontageFinished();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Ember|Animation")
	TObjectPtr<UAnimMontage> ConsumeItemMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Ember|Animation")
	float DefaultAnimationRate = 1.f;
	
	FGameplayEventData CachedGameplayEventData;
};
