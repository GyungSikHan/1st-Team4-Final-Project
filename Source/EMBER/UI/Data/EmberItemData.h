// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EmberItemData.generated.h"

class UItemTemplate;

UCLASS(BlueprintType, Const)
class EMBER_API UEmberItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	static const UEmberItemData& Get();

public:
/* Editor */
#if WITH_EDITORONLY_DATA
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
#endif

public:
	const UItemTemplate& FindItemTemplateByID(int32 ItemTemplateID) const;
	int32 FindItemTemplateIDByClass(TSubclassOf<UItemTemplate> ItemTemplateClass) const;
	
	void GetAllItemTemplateClasses(TArray<TSubclassOf<UItemTemplate>>& OutItemTemplateClasses) const;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TMap<int32, TSubclassOf<UItemTemplate>> ItemTemplateIDToClass;

	UPROPERTY()
	TMap<TSubclassOf<UItemTemplate>, int32> ItemTemplateClassToID;

private:
	UPROPERTY()
	TArray<TSubclassOf<UItemTemplate>> WeaponItemTemplateClasses;

	UPROPERTY()
	TArray<TSubclassOf<UItemTemplate>> ArmorItemTemplateClasses;
};
