// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemFragment_Equipable_Armor.h"

#include "UObject/ObjectSaveContext.h"

UItemFragment_Equipable_Armor::UItemFragment_Equipable_Armor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	EquipmentType = EEquipmentType::Armor;
}

void UItemFragment_Equipable_Armor::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	for (int i = 0; i < RarityStatRangeSets.Num(); i++)
	{
		TArray<FRarityStatRange>& RarityStatRanges = RarityStatRangeSets[i].RarityStatRanges;
		RarityStatRanges.SetNum((int32)EItemRarity::Count);
		for (int32 j = 0; j < RarityStatRanges.Num(); j++)
		{
			RarityStatRanges[j].Rarity = (EItemRarity)j;
		}
	}
}

void UItemFragment_Equipable_Armor::OnInstanceCreated(UItemInstance* ItemInstance) const
{
	Super::OnInstanceCreated(ItemInstance);

	AddStatTagStack(ItemInstance, RarityStatRangeSets);
}
