// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/AbilitySystem/Abilities/EmberGameplayAbility.h"
#include "EmberGameplayAbility_BodyTemperature.generated.h"

class UEmberAbilityTask_WeatherCondition;

UCLASS()
class EMBER_API UBodyTemperatureData : public UObject
{
	GENERATED_BODY()

public:
	FGameplayTag EventTag;
	float IntervalRate = 1.f;
	float Amount = 0.f;
};

UCLASS()
class EMBER_API UEmberGameplayAbility_BodyTemperature : public UEmberGameplayAbility
{
	GENERATED_BODY()

public:
	UEmberGameplayAbility_BodyTemperature(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	void OnChangeBodyTemperature(FGameplayEventData TriggerEventData);
	
private:
	/** Tick 주기 (초당 N회 업데이트) */
	UPROPERTY(EditDefaultsOnly, Category="BodyTemperature|Info")
	float DecreaseIntervalRate;

	/** Tick마다 감소할 체온량 */
	UPROPERTY(EditDefaultsOnly, Category="BodyTemperature|Info")
	float DecreaseAmount;

	UPROPERTY()
	UEmberAbilityTask_WeatherCondition* WeatherCondition;
};
