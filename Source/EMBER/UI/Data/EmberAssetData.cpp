// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberAssetData.h"

#include "System/EmberAssetManager.h"
#include "UObject/ObjectSaveContext.h"

const UEmberAssetData& UEmberAssetData::Get()
{
	return UEmberAssetManager::Get().GetAssetData();
}

void UEmberAssetData::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);

	AssetNameToPath.Empty();

	AssetGroupNameToSet.KeySort([](const FName& A, const FName& B)
	{
		return (A.Compare(B) < 0);
	});
	
	for (const auto& Pair : AssetGroupNameToSet)
	{
		const FAssetSet& AssetSet = Pair.Value;
		for (FAssetEntry AssetEntry : AssetSet.AssetEntries)
		{
			AssetNameToPath.Emplace(AssetEntry.AssetName, AssetEntry.AssetPath);
		}
	}
}

FSoftObjectPath UEmberAssetData::GetAssetPathByName(const FName& AssetName) const
{
	const FSoftObjectPath* AssetPath = AssetNameToPath.Find(AssetName);
	ensureAlwaysMsgf(AssetPath, TEXT("Can't find Asset Path from Asset Name [%s]."), *AssetName.ToString());
	return *AssetPath;
}
