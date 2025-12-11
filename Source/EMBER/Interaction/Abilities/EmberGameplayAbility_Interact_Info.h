// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interaction/EmberInteractionInfo.h"
#include "System/AbilitySystem/Abilities/EmberGameplayAbility.h"
#include "EmberGameplayAbility_Interact_Info.generated.h"

class IEmberInteractable;

UCLASS()
class EMBER_API UEmberGameplayAbility_Interact_Info : public UEmberGameplayAbility
{
	GENERATED_BODY()

public:
	UEmberGameplayAbility_Interact_Info(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UFUNCTION()
	bool InitializeAbility(AActor* TargetActor);

protected:
	UPROPERTY(BlueprintReadOnly)
	TScriptInterface<IEmberInteractable> Interactable;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> InteractableActor;

	UPROPERTY(BlueprintReadOnly)
	FEmberInteractionInfo InteractionInfo;
};
