// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/AbilitySystem/Abilities/EmberGameplayAbility.h"
#include "EmberGameplayAbility_HitReact.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UEmberGameplayAbility_HitReact : public UEmberGameplayAbility
{
	GENERATED_BODY()

public:
	UEmberGameplayAbility_HitReact(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	void OnNetSync();

	UFUNCTION()
	void OnMontageFinished();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Ember|HitReact")
	TSubclassOf<UCameraShakeBase> HitReactCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category="Ember|HitReact")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
