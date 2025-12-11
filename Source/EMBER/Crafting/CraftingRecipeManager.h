#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "CraftingRecipeManager.generated.h"

class UItemTemplate;

UENUM(BlueprintType)
enum class EStationType : uint8
{
    None,
    CraftingTable,
    Furnace,
    WeaponTable,
    ClothingTable,
    Campfire
};

USTRUCT(BlueprintType)
struct FCraftingRecipeRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText RecipeDisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UItemTemplate> ItemTemplateClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FGameplayTag, int32> Ingredients;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTagContainer AllowedMainMaterialTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RequiredMainMaterialCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CraftingTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCraftableByCharacter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTagContainer RequiredStationTags;

    FCraftingRecipeRow()
        : ItemTemplateClass(nullptr)
        , RequiredMainMaterialCount(0)
        , CraftingTime(0.f)
        , bCraftableByCharacter(false)
    {
    }
};

UCLASS(BlueprintType)
class EMBER_API UCraftingRecipeManager : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
    UDataTable* RecipeDataTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
    TMap<int32, FGameplayTagContainer> ItemIDToMaterialTagMap;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
    TMap<FGameplayTag, TSubclassOf<UItemTemplate>> RepresentativeItemForTag;

    const FCraftingRecipeRow* GetRecipeRowByOutputItemID(int32 TemplateID) const;
    
    void GetAllRecipeRows(TArray<FCraftingRecipeRow*>& OutRows) const;

    UFUNCTION(BlueprintPure, Category = "Item Data")
    FGameplayTagContainer GetMaterialTagsForItem(int32 ItemTemplateID) const;
    
    UFUNCTION(BlueprintPure, Category = "Crafting")
    TSubclassOf<UItemTemplate> GetRepresentativeItemForTag(const FGameplayTag& MaterialTag) const;
    
    virtual void PostLoad() override;
};