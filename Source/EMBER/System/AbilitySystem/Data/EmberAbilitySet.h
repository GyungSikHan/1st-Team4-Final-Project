// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayAbilitySpec.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "EmberAbilitySet.generated.h"

class UEmberAbilitySystemComponent;
class UEmberGameplayAbility;
class UAttributeSet;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FEmberAbilitySet_GameplayEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	float EffectLevel = 1.0f;
};

USTRUCT(BlueprintType)
struct FEmberAbilitySet_GameplayAbility
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UEmberGameplayAbility> Ability;
	
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;
	
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct FEmberAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void TakeFromAbilitySystem(UEmberAbilitySystemComponent* EmberASC);

protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;
	
	UPROPERTY()
	TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;
};

UCLASS(BlueprintType, Const)
class EMBER_API UEmberAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UEmberAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	void GiveToAbilitySystem(UEmberAbilitySystemComponent* EmberASC, FEmberAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities",  meta=(TitleProperty=Ability))
	TArray<FEmberAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	// Gameplay effects to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta=(TitleProperty=GameplayEffect))
	TArray<FEmberAbilitySet_GameplayEffect> GrantedGameplayEffects;
};
