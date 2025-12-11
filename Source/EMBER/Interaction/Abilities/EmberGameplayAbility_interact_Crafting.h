// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmberGameplayAbility_Interact_Object.h"
#include "EmberGameplayAbility_interact_Crafting.generated.h"

class UCommonActivatableWidget;

UCLASS()
class EMBER_API UEmberGameplayAbility_interact_Crafting : public UEmberGameplayAbility_Interact_Object
{
	GENERATED_BODY()

public:
	UEmberGameplayAbility_interact_Crafting(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UFUNCTION()
	void OnAfterPushWidget(UCommonActivatableWidget* InPushedWidget);

protected:
	UPROPERTY(EditDefaultsOnly, Category="Interaction|Info")
	TSoftClassPtr<UCommonActivatableWidget> WidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="Interaction|Info", meta=(Categories ="UI.Layer"))
	FGameplayTag WidgetLayerTag;

private:
	UPROPERTY()
	TObjectPtr<UCommonActivatableWidget> PushedWidget;
};
