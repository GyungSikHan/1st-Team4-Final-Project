// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interaction/EmberInteractionInfo.h"
#include "System/AbilitySystem/Abilities/EmberGameplayAbility.h"
#include "EmberGameplayAbility_Interact.generated.h"

USTRUCT(BlueprintType)
struct FEmberInteractionMessage
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> Instigator = nullptr;

	UPROPERTY(BlueprintReadWrite)
	bool bShouldRefresh = false;

	UPROPERTY(BlueprintReadWrite)
	bool bSwitchActive = false;

	UPROPERTY(BlueprintReadWrite)
	FEmberInteractionInfo InteractionInfo = FEmberInteractionInfo();
};

UCLASS()
class EMBER_API UEmberGameplayAbility_Interact : public UEmberGameplayAbility
{
	GENERATED_BODY()

public:
	UEmberGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer);
	
protected:
	//~UGameplayAbility Overrides
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	//~End of UGameplayAbility Overrides

protected:
	UFUNCTION(BlueprintCallable)
	void UpdateInteractions(const FEmberInteractionInfo& InteractionInfo);

	
protected:
	UFUNCTION(BlueprintCallable)
	void TriggerInteraction();
	
private:
	void WaitInputStart();

	UFUNCTION()
	void OnInputStart();

protected:
	UPROPERTY(BlueprintReadWrite)
	FEmberInteractionInfo CurrentInteractionInfo;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Ember|Interaction")
	float InteractionTraceRange = 150.f;

	UPROPERTY(EditDefaultsOnly, Category="Ember|Interaction")
	float InteractionTraceRate = 0.1f;
	
	UPROPERTY(EditDefaultsOnly, Category="Ember|Interaction")
	float InteractionScanRange = 500.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Ember|Interaction")
	float InteractionScanRate = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category="Ember|Interaction")
	bool bShowTraceDebug = false;

	UPROPERTY(EditDefaultsOnly, Category="Ember|Interaction")
	TSoftClassPtr<UUserWidget> DefaultInteractionWidgetClass;
};
