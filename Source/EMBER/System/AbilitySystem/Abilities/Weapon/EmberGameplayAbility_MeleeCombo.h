// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmberGameplayAbility_MeleeAttack.h"
#include "EmberGameplayAbility_MeleeCombo.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UEmberGameplayAbility_MeleeCombo : public UEmberGameplayAbility_MeleeAttack
{
	GENERATED_BODY()

public:
	UEmberGameplayAbility_MeleeCombo(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void HandleMontageEvent(FGameplayEventData Payload) override;

private:
	void WaitInputContinue();
	void WaitInputStop();
	
private:
	UFUNCTION()
	void OnInputReleased(float TimeHeld);

	UFUNCTION()
	void OnInputStart();

	UFUNCTION()
	void OnInputCancel();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Melee Combo")
	TSubclassOf<UEmberGameplayAbility_MeleeCombo> NextAbilityClass;
	
private:
	bool bInputPressed = false;
	bool bInputReleased = false;
};
