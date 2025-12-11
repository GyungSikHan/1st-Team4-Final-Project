// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFlag.h"
#include "GameplayTagContainer.h"
#include "Item/ItemTemplate.h"
#include "UObject/Object.h"
#include "ItemFragment_Equipable.generated.h"

class UEmberAbilitySet;

USTRUCT(BlueprintType)
struct FRarityStat
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly)
	EItemRarity Rarity = EItemRarity::Common;

	UPROPERTY(EditDefaultsOnly)
	int32 Value = 0;
};

USTRUCT(BlueprintType)
struct FRarityStatSet
{
	GENERATED_BODY()

public:
	FRarityStatSet();
	
public:
	UPROPERTY(EditDefaultsOnly, meta=(Categories="SetByCaller"))
	FGameplayTag StatTag;
	
	UPROPERTY(EditDefaultsOnly, EditFixedSize)
	TArray<FRarityStat> RarityStats;
};

USTRUCT(BlueprintType)
struct FRarityStatRange
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly)
	EItemRarity Rarity = EItemRarity::Common;
	
	UPROPERTY(EditDefaultsOnly)
	int32 MinValue = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxValue = 0;
};

USTRUCT(BlueprintType)
struct FRarityStatRangeSet
{
	GENERATED_BODY()

public:
	FRarityStatRangeSet();
	
public:
	UPROPERTY(EditDefaultsOnly, meta=(Categories="ItemAttribute"))
	FGameplayTag StatTag;
	
	UPROPERTY(EditDefaultsOnly, EditFixedSize)
	TArray<FRarityStatRange> RarityStatRanges;
};

UCLASS(Abstract, Const)
class EMBER_API UItemFragment_Equipable : public UItemFragment
{
	GENERATED_BODY()

public:
	UItemFragment_Equipable(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	void AddStatTagStack(UItemInstance* ItemInstance, const TArray<FRarityStatRangeSet>& RarityStatRangeSets) const;

public:
	UPROPERTY(EditDefaultsOnly, Category="Ember|Equipable")
	EEquipmentType EquipmentType = EEquipmentType::Count;

	UPROPERTY(EditDefaultsOnly, Category="Ember|Equipable")
	TObjectPtr<const UEmberAbilitySet> BaseAbilitySet;
};
