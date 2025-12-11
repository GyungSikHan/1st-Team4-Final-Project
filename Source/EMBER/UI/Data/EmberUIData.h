// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFlag.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "EmberUIData.generated.h"

class UInventorySlotWidget;
class UInventoryValidWidget;
class UInventoryEntryWidget;
class UItemHoverWidget;
class UItemDragWidget;
class UCommonActivatableWidget;
class UEquipmentEntryWidget;

USTRUCT(BlueprintType)
struct FEmberHUDInfo
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (Categories = "UI.Layer"))
	FGameplayTag HUDWidgetTag;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCommonActivatableWidget> HUDWidgetClass;
};

USTRUCT(BlueprintType)
struct FEmberItemRarityInfoEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, meta=(HideAlphaChannel))
	FColor Color = FColor::Black;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> EntryTexture;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> HoverTexture;
};

UCLASS()
class EMBER_API UEmberUIData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:	
	static const UEmberUIData& Get();

public:
	UTexture2D* GetHoverRarityTexture(EItemRarity ItemRarity) const;
	FColor GetRarityColor(EItemRarity ItemRarity) const;

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventorySlotWidget> InventorySlotWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryValidWidget> InventoryValidWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryEntryWidget> InventoryEntryWidgetClass;

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemHoverWidget> ItemHoverWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemDragWidget> ItemDragWidgetClass;

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UEquipmentEntryWidget> EquipmentEntryWidgetClass;
	
public:
	UPROPERTY(EditDefaultsOnly)
	FIntPoint UnitInventorySlotSize = FIntPoint::ZeroValue;

	UPROPERTY(EditDefaultsOnly)
	FEmberHUDInfo EmberHUDInfo;
	
private:
	UPROPERTY(EditDefaultsOnly, meta=(ArraySizeEnum="EItemRarity"))
	FEmberItemRarityInfoEntry RarityInfoEntries[(int32)EItemRarity::Count];
};
