// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/AbilitySystem/Abilities/EmberGameplayAbility.h"
#include "EmberGameplayAbility_Dash_Active.generated.h"


UCLASS()
class EMBER_API UEmberGameplayAbility_Dash_Active : public UEmberGameplayAbility
{
	GENERATED_BODY()

public:
	UEmberGameplayAbility_Dash_Active(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	UAnimMontage* SelectDirectionalMontage(EEmberDirection MovementDirection) const;
	
private:
	UFUNCTION()
	void OnMontageFinished();

	UFUNCTION()
	void OnDashFinished();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Ember|Dash Active")
	float DashStrength = 450.f;

	UPROPERTY(EditDefaultsOnly, Category="Ember|Dash Active")
	float DashDuration = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category="Ember|Dash Active")
	TObjectPtr<UAnimMontage> BackwardMontage;

	UPROPERTY(EditDefaultsOnly, Category="Ember|Dash Active")
	TObjectPtr<UAnimMontage> LeftMontage;

	UPROPERTY(EditDefaultsOnly, Category="Ember|Dash Active")
	TObjectPtr<UAnimMontage> RightMontage;
};
