// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/AssetManager.h"
#include "Input/Data/EmberInputConfig.h"
#include "UI/Data/EmberAssetData.h"
#include "UI/Data/EmberItemData.h"
#include "UI/Data/EmberUIData.h"
#include "UI/Data/EmberPawnData.h"
#include "Crafting/CraftingRecipeManager.h"

#include "EmberAssetManager.generated.h"

UCLASS(Config=Game)
class EMBER_API UEmberAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UEmberAssetManager();
	
protected:
	//~UAssetManager Overrides
	virtual void StartInitialLoading() override;
	//~End of UAssetManager Overrides

public:
	static UEmberAssetManager& Get();

	template<typename AssetType>
	static AssetType* GetAssetByPath(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);
	
	template<typename AssetType>
	static TSubclassOf<AssetType> GetSubclassByPath(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);
	
	template<typename AssetType>
	static TSubclassOf<AssetType> GetSubclassByName(const FName& AssetName, bool bKeepInMemory = true);
	
	const UEmberUIData& GetUIData();
	const UEmberItemData& GetItemData();
	const UEmberAssetData& GetAssetData();
	const UEmberPawnData& GetPawnData();
	const UCraftingRecipeManager& GetRecipeData();
	
protected:
	UPrimaryDataAsset* LoadGameDataOfClass(TSubclassOf<UPrimaryDataAsset> DataClass, const TSoftObjectPtr<UPrimaryDataAsset>& DataClassPath, FPrimaryAssetType PrimaryAssetType);
	
	template <typename GameDataClass>
	const GameDataClass& GetOrLoadTypedGameData(const TSoftObjectPtr<GameDataClass>& DataPath)
	{
		if (TObjectPtr<UPrimaryDataAsset> const * pResult = GameDataMap.Find(GameDataClass::StaticClass()))
		{
			return *CastChecked<GameDataClass>(*pResult);
		}

		UE_LOG(LogTemp, Log, TEXT("Loading GameData from path: %s"), *DataPath.ToString());
		
		return *CastChecked<const GameDataClass>(LoadGameDataOfClass(GameDataClass::StaticClass(), DataPath, GameDataClass::StaticClass()->GetFName()));
	}

	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);

	// Thread safe way of adding a loaded asset to keep in memory.
	void AddLoadedAsset(const UObject* Asset);
	
protected:
	UPROPERTY(Config)
	TSoftObjectPtr<UEmberUIData> UIDataPath;

	UPROPERTY(Config)
	TSoftObjectPtr<UEmberItemData> ItemDataPath;

	UPROPERTY(Config)
	TSoftObjectPtr<UEmberAssetData> AssetDataPath;

	UPROPERTY(Config)
	TSoftObjectPtr<UEmberInputConfig> InputConfigPath;

	UPROPERTY(Config)
	TSoftObjectPtr<UEmberPawnData> PawnDataPath;

	UPROPERTY(Config)
	TSoftObjectPtr<UCraftingRecipeManager> RecipeDataPath;
	
	UPROPERTY(Transient)
	TMap<TObjectPtr<UClass>, TObjectPtr<UPrimaryDataAsset>> GameDataMap;

private:
	// Assets loaded and tracked by the asset manager.
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;
	
	// Used for a scope lock when modifying the list of load assets.
	FCriticalSection LoadedAssetsCritical;
};

template <typename AssetType>
AssetType* UEmberAssetManager::GetAssetByPath(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetPointer.Get();
		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}

		if (LoadedAsset && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}

	return LoadedAsset;
}

template <typename AssetType>
TSubclassOf<AssetType> UEmberAssetManager::GetSubclassByPath(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
		}

		if (LoadedSubclass && bKeepInMemory)
		{
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}

template <typename AssetType>
TSubclassOf<AssetType> UEmberAssetManager::GetSubclassByName(const FName& AssetName, bool bKeepInMemory)
{
	const UEmberAssetData& AssetData = Get().GetAssetData();
	const FSoftObjectPath& AssetPath = AssetData.GetAssetPathByName(AssetName);
	
	FString AssetPathString = AssetPath.GetAssetPathString();
	AssetPathString.Append(TEXT("_C"));

	FSoftClassPath ClassPath(AssetPathString);
	TSoftClassPtr<AssetType> ClassPtr(ClassPath);
	return GetSubclassByPath<AssetType>(ClassPtr, bKeepInMemory);
}
