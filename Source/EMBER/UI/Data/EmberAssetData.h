// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "EmberAssetData.generated.h"

USTRUCT()
struct FAssetEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FName AssetName;
	
	UPROPERTY(EditDefaultsOnly)
	FSoftObjectPath AssetPath;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FName> AssetLabels;
};

USTRUCT()
struct FAssetSet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FAssetEntry> AssetEntries;
};

/**
 * 
 */
UCLASS()
class EMBER_API UEmberAssetData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	static const UEmberAssetData& Get();

protected:
/* Editor */
//#if WITH_EDITORONLY_DATA
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
//#endif
	
public:
	FSoftObjectPath GetAssetPathByName(const FName& AssetName) const;

	UPROPERTY(EditDefaultsOnly)
	TMap<FName, FAssetSet> AssetGroupNameToSet;
	
	UPROPERTY()
	TMap<FName, FSoftObjectPath> AssetNameToPath;
};
