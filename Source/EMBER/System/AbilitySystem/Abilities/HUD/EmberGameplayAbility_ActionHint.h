// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EmberActionHintInfo.h"
#include "System/AbilitySystem/Abilities/EmberGameplayAbility.h"
#include "EmberGameplayAbility_ActionHint.generated.h"

USTRUCT(BlueprintType)
struct FEmberActionHintMessage
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	bool bShouldRefresh = false;

	UPROPERTY(BlueprintReadWrite)
	FEmberActionHintInfo ActionHintInfo = FEmberActionHintInfo();
};

UCLASS()
class EMBER_API UEmberGameplayAbility_ActionHint : public UEmberGameplayAbility
{
	GENERATED_BODY()

public:
	UEmberGameplayAbility_ActionHint(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~UGameplayAbility Overrides
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End of UGameplayAbility Overrides

private:
	UFUNCTION()
	void OnTagChanged(const FGameplayTag ChangedTag, int32 NewCount);

	void SetActionHintInfo(FGameplayTag InAbilityTag);
	void ClearActionHintInfo();

	void UpdateActionHint();
	
private:
	UPROPERTY(EditDefaultsOnly)
	TArray<FEmberActionHintInfo> ActionHintInfos;

	UPROPERTY()
	FEmberActionHintInfo CurrentActionHintInfo;
	
	TMap<FGameplayTag, FDelegateHandle> EventHandleMap;
};
