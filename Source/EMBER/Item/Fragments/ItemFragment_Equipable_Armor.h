// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFlag.h"
#include "ItemFragment_Equipable.h"
#include "ItemFragment_Equipable_Armor.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UItemFragment_Equipable_Armor : public UItemFragment_Equipable
{
	GENERATED_BODY()
public:
	UItemFragment_Equipable_Armor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
//#if WITH_EDITORONLY_DATA
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
//#endif // WITH_EDITORONLY_DATA
	
public:
	virtual void OnInstanceCreated(UItemInstance* ItemInstance) const override;
	
public:
	UPROPERTY(EditDefaultsOnly)
	EArmorType ArmorType = EArmorType::Count;
	
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<USkeletalMesh> ArmorMesh;

	UPROPERTY(EditDefaultsOnly, meta=(ForceInlineRow))
	TArray<FRarityStatRangeSet> RarityStatRangeSets;
};
