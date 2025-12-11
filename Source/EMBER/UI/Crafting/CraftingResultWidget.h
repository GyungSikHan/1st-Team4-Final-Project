#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFlag.h"
#include "UI/Common/EmberActivatableWidget.h"
#include "Crafting/CraftingRecipeManager.h"
#include "CraftingResultWidget.generated.h"

class UTextBlock;
class UImage;
class UVerticalBox;
class UButton;
struct FCraftingRecipeRow;
class UItemTemplate;
class UCraftingResultIngredientLineEntry;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCraftRequested);

UCLASS()
class EMBER_API UCraftingResultWidget : public UEmberActivatableWidget
{
    GENERATED_BODY()

public:
    UCraftingResultWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, Category = "CraftingResult")
    void SetRecipeDetails(UCraftingRecipeManager* RecipeManager, const FCraftingRecipeRow& InRecipeRow, const TMap<FGameplayTag, int32>& PlayerOwnedIngredients, int32 CraftingAmount);

    UFUNCTION(BlueprintCallable, Category = "CraftingResult")
    void UpdateRarityDisplay(const FCraftingRecipeRow& TargetRecipeData, const TMap<EItemRarity, float>& InRarityChances);
    
    UFUNCTION(BlueprintCallable, Category = "CraftingResult")
    void ClearDetails();

    UPROPERTY(BlueprintAssignable, Category = "CraftingResult")
    FOnCraftRequested OnCraftRequested;

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    
    UPROPERTY(BlueprintReadOnly, Category = "CraftingResult", meta = (BindWidget))
    TObjectPtr<UButton> Button_Craft;
    
    UPROPERTY(BlueprintReadOnly, Category = "CraftingResult", meta = (BindWidget))
    TObjectPtr<UVerticalBox> IngredientsDisplayBox;
    
    UPROPERTY(EditDefaultsOnly, Category = "CraftingResult|UI")
    TSubclassOf<UCraftingResultIngredientLineEntry> ResultIngredientLineEntryClass;

    UPROPERTY(BlueprintReadOnly, Category = "CraftingResult", meta = (BindWidgetOptional))
    TObjectPtr<UTextBlock> ResultItemNameText; 
    UPROPERTY(BlueprintReadOnly, Category = "CraftingResult", meta = (BindWidgetOptional))
    TObjectPtr<UImage> ItemIconDisplay;
    UPROPERTY(BlueprintReadOnly, Category = "CraftingResult", meta = (BindWidgetOptional))
    TObjectPtr<UTextBlock> ItemDescriptionText;
    UPROPERTY(BlueprintReadOnly, Category = "CraftingResult", meta = (BindWidgetOptional))
    TObjectPtr<UTextBlock> InfoTextDisplay; 

    UFUNCTION(BlueprintImplementableEvent, Category = "CraftingResult", meta = (DisplayName = "OnUpdateIconAndDescription_BP"))
    void K2_OnUpdateIconAndDescription(const UItemTemplate* ItemTemplateData);

private:
    bool PopulateIngredientsDisplay(UCraftingRecipeManager* RecipeManager, const FCraftingRecipeRow& RecipeData, const TMap<FGameplayTag, int32>& PlayerOwnedIngredients, int32 CraftingAmount);
    
    UFUNCTION()
    void HandleCraftButtonClicked();
    
    const UItemTemplate* GetTemplateFromRecipeRow(const FCraftingRecipeRow& RecipeRow) const;

    const FCraftingRecipeRow* CurrentTargetRecipeRowPtr_EditorOnly;
    TMap<EItemRarity, float> CurrentRarityChances_Cache; 
};