// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "System/AbilitySystem/Abilities/Environment/FEmberBodyTemperatureQuery.h"
#include "EmberAbilityTask_WeatherCondition.generated.h"

class AEmberPlayerCharacter;

UCLASS()
class EMBER_API UEmberAbilityTask_WeatherCondition : public UAbilityTask
{
	GENERATED_BODY()
public:
	UEmberAbilityTask_WeatherCondition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta=(HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly="true"))
	static UEmberAbilityTask_WeatherCondition* WaitForDecreaseBodyTemperature(UGameplayAbility* OwningAbility, FEmberBodyTemperatureQuery BodyTemperatureQuery, float InDecreaseIntervalRate = 1.f, float InDecreaseAmount = 0.1f);

protected:
	//~UAbilityTask Overrides
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
	//~End of AbilityTask Overrides

private:
	void PerformTask();
	
private:
	UPROPERTY()
	AEmberPlayerCharacter* TargetCharacter;
	
	/* Tick 주기 (초당 N회 업데이트) */
	float DecreaseIntervalRate;
	
	/* Tick마다 감소할 체온량 */
	float DecreaseAmount;

	/* Timer */
	FTimerHandle DecreaseBodyTemperatureTimerHandle;
};
