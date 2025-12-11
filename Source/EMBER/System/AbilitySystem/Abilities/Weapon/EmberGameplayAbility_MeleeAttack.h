// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameData.h"
#include "GameplayAbility_Equipment.h"
#include "System/AbilitySystem/Abilities/EmberGameplayAbility.h"
#include "EmberGameplayAbility_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UEmberGameplayAbility_MeleeAttack : public UGameplayAbility_Equipment
{
	GENERATED_BODY()

public:
	UEmberGameplayAbility_MeleeAttack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~UGameplayAbility Overrides
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void HandleMontageEvent(FGameplayEventData Payload);
	//~End of UGameplayAbility Overrides

private:
	UFUNCTION()
	void OnHitTarget(FGameplayEventData Payload);
	
	UFUNCTION()
	void OnMontageEventTriggered(FGameplayEventData Payload);
	
	UFUNCTION(BlueprintCallable)
	void OnMontageFinished();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Ember|Melee Attack")
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category="Ember|Equipment")
	float DefaultAttackRate = 1.f;

private:
	FDamageData HittedData; 
};
