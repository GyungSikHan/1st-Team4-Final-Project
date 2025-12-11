// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFlag.h"
#include "ItemFragment_Equipable_Attachment.h"
#include "ItemFragment_Equipable_Utility.generated.h"

USTRUCT()
struct FConsumableEffect
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, meta=(Categories="ItemAttribute.Comsumable"))
	FGameplayTag EffectTag;

	UPROPERTY(EditDefaultsOnly)
	float Magnitude = 0.f;
};

UCLASS()
class EMBER_API UItemFragment_Equipable_Utility : public UItemFragment_Equipable_Attachment
{
	GENERATED_BODY()

public:
	UItemFragment_Equipable_Utility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
public:
	UPROPERTY(EditDefaultsOnly)
	EUtilityType UtilityType = EUtilityType::Count;

	UPROPERTY(EditDefaultsOnly)
	TArray<FRarityStatRangeSet> RarityStatRangeSets;

	UPROPERTY(EditDefaultsOnly)
	TArray<FConsumableEffect> ConsumableEffects;
};
