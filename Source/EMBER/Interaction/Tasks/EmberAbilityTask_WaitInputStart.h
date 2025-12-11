// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "EmberAbilityTask_WaitInputStart.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputStartDelegate);

UCLASS()
class EMBER_API UEmberAbilityTask_WaitInputStart : public UAbilityTask
{
	GENERATED_BODY()

public:
	UEmberAbilityTask_WaitInputStart(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable)
	static UEmberAbilityTask_WaitInputStart* WaitInputStart(UGameplayAbility* OwningAbility);

public:
	//~UAbilityTask Overrides
	virtual void Activate() override;
	//~End of UAbilityTask Overrides

public:
	UFUNCTION()
	void OnStartCallback();
	
public:
	UPROPERTY(BlueprintAssignable)
	FInputStartDelegate OnStart;

protected:
	FDelegateHandle DelegateHandle;
};
