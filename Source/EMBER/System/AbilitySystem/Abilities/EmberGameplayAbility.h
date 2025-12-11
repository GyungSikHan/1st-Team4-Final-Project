// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EmberGameplayAbility.generated.h"

class UEmberAbilitySystemComponent;
class UEmberLocalPlayer;
class AEmberPlayerCharacter;
class AEmberPlayerController;
class UEmberAbilityCost;

UENUM(BlueprintType)
enum class EEmberAbilityActivationPolicy : uint8
{
	Manual,
	
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.
	OnSpawn
};

UCLASS()
class EMBER_API UEmberGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEmberGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

	UFUNCTION(BlueprintCallable, Category = "Ember|Ability")
	UEmberAbilitySystemComponent* GetEmberAbilitySystemComponentFromActorInfo() const;
	
	UFUNCTION(BlueprintCallable, Category = "Ember|Ability")
	AEmberPlayerCharacter* GetEmberCharacterFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Ember|Ability")
	AEmberPlayerController* GetEmberPlayerControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Ember|Ability")
	AController* GetControllerFromActorInfo() const;
	
	UFUNCTION(BlueprintCallable, Category = "Ember|Ability")
	UEmberLocalPlayer* GetEmberLocalPlayerFromActorInfo() const;
	
	EEmberAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

protected:
	//~UGameplayAbility interface
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual bool DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	UFUNCTION(BlueprintCallable, Category="Ember|Ability")
	void GetMovementDirection(EEmberDirection& OutDirection, FVector& OutMovementVector) const;
	
protected:
	// Defines how this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ember|Ability Activation")
	EEmberAbilityActivationPolicy ActivationPolicy;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = Costs)
	TArray<TObjectPtr<UEmberAbilityCost>> AdditionalCosts;
};
