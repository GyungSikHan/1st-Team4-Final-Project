#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Crafting/CraftingRecipeManager.h"
#include "GameFlag.h"
#include "CraftingSystem.generated.h"

class UInventoryManagerComponent;
class UItemTemplate;
class UItemInstance;
class ACraftingBuilding;
class AEmberPlayerCharacter;
class UCraftingWidget;

USTRUCT(BlueprintType)
struct FCraftingResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category="CraftingResult")
    bool bWasSuccessful;

    UPROPERTY(BlueprintReadOnly, Category="CraftingResult")
    TSubclassOf<UItemTemplate> ItemTemplateClass;

    UPROPERTY(BlueprintReadOnly, Category="CraftingResult")
    EItemRarity Rarity;

    FCraftingResult()
        : bWasSuccessful(false), ItemTemplateClass(nullptr), Rarity(EItemRarity::Common)
    {}
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EMBER_API UCraftingSystem : public UActorComponent
{
    GENERATED_BODY()

public:
    UCraftingSystem();

protected:
    virtual void InitializeComponent() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Crafting")
    FCraftingResult Client_PreCraftCheck(AEmberPlayerCharacter* Player, const FCraftingRecipeRow& Recipe, const TMap<FGameplayTag, int32>& InSelectedMainIngredients);

    UFUNCTION(BlueprintCallable, Category = "Crafting")
    bool RequestServerCraft(AEmberPlayerCharacter* Player, ACraftingBuilding* CraftingStationActor, FName RecipeRowName, const TMap<FGameplayTag, int32>& InSelectedMainIngredients);

    UFUNCTION(BlueprintCallable, Category = "Crafting")
    bool CanCraftRecipeAtStation(const FCraftingRecipeRow& Recipe, const FGameplayTag& StationTag) const;

    UFUNCTION(BlueprintCallable, Category = "Crafting")
    bool HasRequiredMaterials(AEmberPlayerCharacter* Player, const FCraftingRecipeRow& Recipe, const TMap<FGameplayTag, int32>& InSelectedMainIngredients) const;

    UFUNCTION(BlueprintCallable, Category = "Crafting")
    TMap<FGameplayTag, int32> AggregateIngredients(AEmberPlayerCharacter* Player) const;

    UFUNCTION(BlueprintCallable, Category = "Crafting")
    TMap<EItemRarity, float> GetRarityProbabilities(const FCraftingRecipeRow& Recipe, const TMap<FGameplayTag, int32>& InSelectedMainIngredients) const;

    UFUNCTION(BlueprintCallable, Category = "Crafting")
    EItemRarity EvaluateItemRarity(const FCraftingRecipeRow& Recipe, const TMap<FGameplayTag, int32>& InSelectedMainIngredients) const;
    
    UFUNCTION(BlueprintPure, Category = "Crafting")
    int32 GetMaterialScore(const FGameplayTag& MaterialTag) const;

    // 소비 가능한 아이템이 인벤토리에 있음을 보장해야함
    bool ConsumeMaterials_Server_Unsafe(AEmberPlayerCharacter* Player, const FCraftingRecipeRow& Recipe, const TMap<FGameplayTag, int32>& InSelectedMainIngredients);

private:
    void InitializeTagMap();
    int32 CalculateScore(const FCraftingRecipeRow& Recipe, const TMap<FGameplayTag, int32>& InSelectedMainIngredients) const;
    
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting System")
    TObjectPtr<UCraftingRecipeManager> RecipeManager;

private:
    TMap<FGameplayTag, int32> TagToScoreMap;
};