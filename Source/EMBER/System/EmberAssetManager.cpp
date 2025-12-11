// Fill out your copyright notice in the Description page of Project Settings.


#include "System/EmberAssetManager.h"

UEmberAssetManager::UEmberAssetManager()
{
}

void UEmberAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	GetItemData();
	GetAssetData();
	GetPawnData();
	GetUIData();
	GetRecipeData();
}

UEmberAssetManager& UEmberAssetManager::Get()
{
	check(GEngine);

	if (UEmberAssetManager* Singleton = Cast<UEmberAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}
	
	return *NewObject<UEmberAssetManager>();
}

const UEmberUIData& UEmberAssetManager::GetUIData()
{
	return GetOrLoadTypedGameData<UEmberUIData>(UIDataPath);
}

const UEmberItemData& UEmberAssetManager::GetItemData()
{
	return GetOrLoadTypedGameData<UEmberItemData>(ItemDataPath);
}

const UEmberAssetData& UEmberAssetManager::GetAssetData()
{
	return GetOrLoadTypedGameData<UEmberAssetData>(AssetDataPath);
}

const UEmberPawnData& UEmberAssetManager::GetPawnData()
{
	return GetOrLoadTypedGameData<UEmberPawnData>(PawnDataPath);
}

const UCraftingRecipeManager& UEmberAssetManager::GetRecipeData()
{
	return GetOrLoadTypedGameData<UCraftingRecipeManager>(RecipeDataPath);
}

UPrimaryDataAsset* UEmberAssetManager::LoadGameDataOfClass(TSubclassOf<UPrimaryDataAsset> DataClass, const TSoftObjectPtr<UPrimaryDataAsset>& DataClassPath, FPrimaryAssetType PrimaryAssetType)
{
	UPrimaryDataAsset* Asset = nullptr;
	
	if (DataClassPath.IsNull())
	{
		return nullptr;
	}
	
	Asset = DataClassPath.LoadSynchronous();
	LoadPrimaryAssetsWithType(PrimaryAssetType);

	if (Asset)
	{
		GameDataMap.Add(DataClass, Asset);
	}

	return Asset;
}

UObject* UEmberAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePtr;

		if (UAssetManager::IsInitialized())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath, false);
		}

		// Use LoadObject if asset manager isn't ready yet.
		return AssetPath.TryLoad();
	}

	return nullptr;
}

void UEmberAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock LoadedAssetsLock(&LoadedAssetsCritical);
		LoadedAssets.Add(Asset);
	}
}
