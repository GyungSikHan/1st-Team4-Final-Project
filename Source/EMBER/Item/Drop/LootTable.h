#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Math/IntPoint.h"
#include "GameFlag.h"
#include "GameplayTagContainer.h"
#include "LootTable.generated.h"

class UItemTemplate;

USTRUCT(BlueprintType)
struct FRarityDropInfo
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot")
    EItemRarity Rarity;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot", meta = (ClampMin = "0.0"))
    float Weight;

    FRarityDropInfo() : Rarity(EItemRarity::Common), Weight(1.0f) {}
};

USTRUCT(BlueprintType)
struct FLootDropItem
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot")
    TSubclassOf<UItemTemplate> ItemTemplateClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot")
    FIntPoint QuantityRange;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float DropChance;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot")
    TArray<FRarityDropInfo> PossibleRarities;

    FLootDropItem() : ItemTemplateClass(nullptr), QuantityRange(1, 1), DropChance(1.0f)
    {
        PossibleRarities.Add(FRarityDropInfo());
    }
};

USTRUCT(BlueprintType)
struct FMonsterLootProfile : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot")
    TArray<FLootDropItem> LootDrops;
};

USTRUCT(BlueprintType)
struct FLootResultData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Result")
    TSubclassOf<UItemTemplate> ItemTemplateClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Result")
    int32 Quantity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Result")
    EItemRarity Rarity;
    
    FLootResultData() : ItemTemplateClass(nullptr), Quantity(0), Rarity(EItemRarity::Common) {}
};

USTRUCT(BlueprintType)
struct FMonsterDiedMessage
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message")
    FName MonsterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message")
    FVector DeathLocation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message")
    TObjectPtr<AActor> KillerActor;
};