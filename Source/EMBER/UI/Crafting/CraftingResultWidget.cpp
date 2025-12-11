#include "UI/Crafting/CraftingResultWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Crafting/CraftingRecipeManager.h"
#include "Item/ItemTemplate.h"
#include "UI/Crafting/CraftingResultIngredientLineEntry.h"

UCraftingResultWidget::UCraftingResultWidget(const FObjectInitializer& ObjectInitializer) 
    : Super(ObjectInitializer)
{
    ResultIngredientLineEntryClass = nullptr;
    Button_Craft = nullptr;
    CurrentTargetRecipeRowPtr_EditorOnly = nullptr;
}

void UCraftingResultWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button_Craft)
    {
        UE_LOG(LogTemp, Warning, TEXT("[DEBUG 1] CraftButton is VALID, binding OnClicked."));
        Button_Craft->OnReleased.RemoveDynamic(this, &UCraftingResultWidget::HandleCraftButtonClicked);
        Button_Craft->OnReleased.AddUniqueDynamic(this, &UCraftingResultWidget::HandleCraftButtonClicked);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[DEBUG 1] FATAL: CraftButton pointer is NULL in CraftingResultWidget!"));
    }
}

void UCraftingResultWidget::NativeDestruct()
{
    Super::NativeDestruct();

    if (Button_Craft)
    {
        Button_Craft->OnReleased.Clear();
    }
}

const UItemTemplate* UCraftingResultWidget::GetTemplateFromRecipeRow(const FCraftingRecipeRow& RecipeRow) const
{
    if (RecipeRow.ItemTemplateClass)
    {
        return RecipeRow.ItemTemplateClass->GetDefaultObject<UItemTemplate>();
    }
    return nullptr;
}

void UCraftingResultWidget::SetRecipeDetails(UCraftingRecipeManager* RecipeManager, const FCraftingRecipeRow& InRecipeRow, const TMap<FGameplayTag, int32>& PlayerOwnedIngredients, int32 CraftingAmount)
{
    if(!RecipeManager) return;
    
    CurrentTargetRecipeRowPtr_EditorOnly = &InRecipeRow;
    const UItemTemplate* OutputTemplate = GetTemplateFromRecipeRow(InRecipeRow);

    if (ResultItemNameText)
    {
        ResultItemNameText->SetText(InRecipeRow.RecipeDisplayName);
    }

    K2_OnUpdateIconAndDescription(OutputTemplate); 

    const bool bCanCraft = PopulateIngredientsDisplay(RecipeManager, InRecipeRow, PlayerOwnedIngredients, CraftingAmount);

    if (Button_Craft)
    {
        Button_Craft->SetIsEnabled(true);
    }

    if (InfoTextDisplay)
    {
        InfoTextDisplay->SetVisibility(ESlateVisibility::Collapsed);
    }
    
    SetVisibility(ESlateVisibility::Visible);
}

void UCraftingResultWidget::UpdateRarityDisplay(const FCraftingRecipeRow& TargetRecipeData, const TMap<EItemRarity, float>& InRarityChances)
{
    CurrentRarityChances_Cache = InRarityChances; 

    bool bIsQualityRecipe = (TargetRecipeData.RequiredMainMaterialCount > 0);
    if (!bIsQualityRecipe) 
    {
        if(InfoTextDisplay) InfoTextDisplay->SetVisibility(ESlateVisibility::Collapsed);
        return; 
    }

    if(InfoTextDisplay)
    {
        FString DisplayStr = TEXT("예상 제작 등급:\n");
        if (CurrentRarityChances_Cache.Num() > 0)
        {
            for (const auto& Pair : CurrentRarityChances_Cache)
            {
                if (Pair.Value > 0.0001f) 
                {
                    FString RarityName = UEnum::GetDisplayValueAsText(Pair.Key).ToString();
                    DisplayStr += FString::Printf(TEXT("- %s: %.2f%%\n"), *RarityName, Pair.Value * 100.0f);
                }
            }
        }
        else
        {
             DisplayStr += TEXT("- 주재료를 선택하세요.");
        }
        InfoTextDisplay->SetText(FText::FromString(DisplayStr));
        InfoTextDisplay->SetVisibility(ESlateVisibility::Visible);
    }
}


void UCraftingResultWidget::ClearDetails()
{
    CurrentTargetRecipeRowPtr_EditorOnly = nullptr;
    if (ResultItemNameText) ResultItemNameText->SetText(FText::GetEmpty());
    if (ItemDescriptionText) ItemDescriptionText->SetText(FText::GetEmpty());
    if (InfoTextDisplay) InfoTextDisplay->SetText(FText::GetEmpty());
    if (IngredientsDisplayBox) IngredientsDisplayBox->ClearChildren();
    
    if (Button_Craft)
    {
        Button_Craft->SetIsEnabled(false);
    }
    
    K2_OnUpdateIconAndDescription(nullptr);
    SetVisibility(ESlateVisibility::Collapsed);
}

bool UCraftingResultWidget::PopulateIngredientsDisplay(UCraftingRecipeManager* RecipeManager, const FCraftingRecipeRow& RecipeData, const TMap<FGameplayTag, int32>& PlayerOwnedIngredients, int32 CraftingAmount)
{
    if (!IngredientsDisplayBox || !ResultIngredientLineEntryClass || !RecipeManager) return false;
    
    IngredientsDisplayBox->ClearChildren();
    IngredientsDisplayBox->SetVisibility(ESlateVisibility::Visible);

    bool bAllIngredientsAvailable = true;

    for (const TPair<FGameplayTag, int32>& RequiredIngredientPair : RecipeData.Ingredients)
    {
       const FGameplayTag& IngredientTag = RequiredIngredientPair.Key;
       const int32 TotalRequiredForBatch = RequiredIngredientPair.Value * CraftingAmount;
       const int32 OwnedAmount = PlayerOwnedIngredients.FindRef(IngredientTag); 
       
       if (OwnedAmount < TotalRequiredForBatch)
       {
           bAllIngredientsAvailable = false;
       }
       
       UCraftingResultIngredientLineEntry* LineEntryWidget = CreateWidget<UCraftingResultIngredientLineEntry>(this, ResultIngredientLineEntryClass);
       if (LineEntryWidget)
       {
           LineEntryWidget->SetLineData(RecipeManager, IngredientTag, OwnedAmount, TotalRequiredForBatch);
           IngredientsDisplayBox->AddChildToVerticalBox(LineEntryWidget);
       }
    }
    
    return bAllIngredientsAvailable;
}

void UCraftingResultWidget::HandleCraftButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("[DEBUG 2] CraftButton CLICKED! Broadcasting OnCraftRequested..."));
    
    if (OnCraftRequested.IsBound())
    {
        OnCraftRequested.Broadcast();
    }
}