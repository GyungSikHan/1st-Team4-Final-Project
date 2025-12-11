// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Data/EmberUIData.h"

#include "System/EmberAssetManager.h"

const UEmberUIData& UEmberUIData::Get()
{
	return UEmberAssetManager::Get().GetUIData();
}

UTexture2D* UEmberUIData::GetHoverRarityTexture(EItemRarity ItemRarity) const
{
	const int32 ItemRarityIndex = (int32)ItemRarity;
	if (ItemRarityIndex < 0 || ItemRarityIndex >= (int32)EItemRarity::Count)
		return nullptr;

	return RarityInfoEntries[ItemRarityIndex].HoverTexture;
}

FColor UEmberUIData::GetRarityColor(EItemRarity ItemRarity) const
{
	const int32 ItemRarityIndex = (int32)ItemRarity;
	if (ItemRarityIndex < 0 || ItemRarityIndex >= (int32)EItemRarity::Count)
		return FColor::Black;

	return RarityInfoEntries[ItemRarityIndex].Color;
}
