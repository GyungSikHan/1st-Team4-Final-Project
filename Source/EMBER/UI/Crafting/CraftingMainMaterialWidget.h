#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "../Common/EmberActivatableWidget.h"
#include "CraftingMainMaterialWidget.generated.h"

class UListView;
class UTextBlock;

USTRUCT(BlueprintType)
struct FSelectableMainMaterialInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaterialInfo")
    FGameplayTag MaterialTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaterialInfo")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaterialInfo")
    TSoftObjectPtr<UTexture2D> Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaterialInfo")
    int32 AvailableCountInInventory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaterialInfo")
    int32 Grade;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaterialInfo")
    int32 CurrentlyStagedCount = 0;

    FSelectableMainMaterialInfo()
        : AvailableCountInInventory(0)
        , Grade(0)
        , CurrentlyStagedCount(0)
    {}
};

USTRUCT(BlueprintType)
struct FSelectedIngredientsMapWrapper
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wrapper")
    TMap<FGameplayTag, int32> IngredientsMap;

    FSelectedIngredientsMapWrapper() {}
    FSelectedIngredientsMapWrapper(const TMap<FGameplayTag, int32>& InMap) : IngredientsMap(InMap) {}
};

UCLASS(BlueprintType)
class USelectableMaterialEntryData : public UObject
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadOnly, Category="Data")
    FSelectableMainMaterialInfo MaterialInfo;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMainMaterialSelectionChanged, const FSelectedIngredientsMapWrapper&, SelectedIngredientsWrapper);

UCLASS()
class EMBER_API UCraftingMainMaterialWidget : public UEmberActivatableWidget
{
    GENERATED_BODY()

public:
    UCraftingMainMaterialWidget(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable, Category = "Crafting|MainMaterialWidget")
    void InitializeSelector(const TArray<FSelectableMainMaterialInfo>& AvailableMaterialsFromInventory, int32 InRequiredTotalCount);

    UFUNCTION(BlueprintCallable, Category = "Crafting|MainMaterialWidget")
    bool TryStageMaterial(FGameplayTag MaterialTag, int32 QuantityToStage = 1);

    UFUNCTION(BlueprintCallable, Category = "Crafting|MainMaterialWidget")
    bool TryUnstageMaterial(FGameplayTag MaterialTag, int32 QuantityToUnstage = 1);

    UFUNCTION(BlueprintCallable, Category = "Crafting|MainMaterialWidget")
    void ClearStagedMaterials();

    const TMap<FGameplayTag, int32>& GetStagedMainIngredientsMap() const;
    
    UFUNCTION(BlueprintPure, Category = "Crafting|MainMaterialWidget")
    int32 GetCurrentStagedTotalCount() const;

    UPROPERTY(BlueprintAssignable, Category = "Crafting|MainMaterialWidget")
    FOnMainMaterialSelectionChanged OnSelectionChanged;

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Crafting|MainMaterialWidget", meta = (BindWidgetOptional))
    UListView* MaterialListView;
    
    UPROPERTY(BlueprintReadOnly, Category = "Crafting|MainMaterialWidget", meta = (BindWidgetOptional))
    UTextBlock* StagedCountText;

    UPROPERTY(BlueprintReadOnly, Category = "Crafting|MainMaterialWidget")
    TArray<FSelectableMainMaterialInfo> DisplayableMaterials;

    UPROPERTY(BlueprintReadOnly, Category = "Crafting|MainMaterialWidget")
    int32 RequiredMainMaterialTotalCount;

    UFUNCTION(BlueprintCallable, Category = "Crafting|MainMaterialWidget")
    void UpdateStagedCountText();

private:
    TMap<FGameplayTag, int32> StagedMainIngredients;
    void BroadcastSelectionChange();
};