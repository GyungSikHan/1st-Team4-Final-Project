// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EMBER.h"
#include "ItemFragment_Equipable_Attachment.h"
#include "ItemFragment_Equipable_Weapon.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UItemFragment_Equipable_Weapon : public UItemFragment_Equipable_Attachment
{
	GENERATED_BODY()

public:
	UItemFragment_Equipable_Weapon(const FObjectInitializer& FObjectInitializer = FObjectInitializer::Get());

public:
	//~UItemFragment Override
	virtual void OnInstanceCreated(UItemInstance* ItemInstance) const;
	//~End of UItemFragment Override

public:
	UPROPERTY(EditDefaultsOnly)
	EWeaponType WeaponType = EWeaponType::Count;

	UPROPERTY(EditDefaultsOnly)
	TArray<FRarityStatRangeSet> RarityStatRangeSets;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	FAttackData AttackInfo;

public:
	//void IncrementMontageIndex();
	FAttackData GetAttackInfo() const;
};
