// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EmberPawnData.generated.h"

class UEmberAbilitySet;
class UEmberInputConfig;
class UEmberAbilityTagRelationshipMapping;
/**
 * 
 */
UCLASS(BlueprintType, Const)
class EMBER_API UEmberPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UEmberPawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
public:
	static const UEmberPawnData& Get();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ember|Abilities")
	TArray<TObjectPtr<UEmberAbilitySet>> AbilitySets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ember|Input")
	TObjectPtr<UEmberInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ember|Abilities")
	TObjectPtr<UEmberAbilityTagRelationshipMapping> TagRelationshipMapping;
};
