#include "UI/Crafting/CraftingResultIngredientLineEntry.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Crafting/CraftingRecipeManager.h"
#include "Item/ItemTemplate.h"
#include "Styling/SlateColor.h"

UCraftingResultIngredientLineEntry::UCraftingResultIngredientLineEntry(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    IngredientIcon = nullptr;
    IngredientNameText = nullptr;
    QuantityRatioText = nullptr;
    LayoutHorizontalBox = nullptr;
}

void UCraftingResultIngredientLineEntry::SetLineData(UCraftingRecipeManager* RecipeManager, FGameplayTag IngredientTag, int32 OwnedCount, int32 RequiredCount)
{
    if (!RecipeManager || !IngredientTag.IsValid())
    {
        SetVisibility(ESlateVisibility::Collapsed);
        return;
    }
    
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);

    TSubclassOf<UItemTemplate> RepItemTemplateClass = RecipeManager->GetRepresentativeItemForTag(IngredientTag);
    if (RepItemTemplateClass)
    {
        const UItemTemplate* ItemTemplateCDO = RepItemTemplateClass->GetDefaultObject<UItemTemplate>();
        if (ItemTemplateCDO)
        {
            if (IngredientIcon)
            {
                IngredientIcon->SetBrushFromTexture(ItemTemplateCDO->IconTexture, true);
                IngredientIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
            }

            if (IngredientNameText)
            {
                IngredientNameText->SetText(ItemTemplateCDO->DisplayName);
            }
        }
    }
    else
    {
        if (IngredientIcon)
        {
            IngredientIcon->SetVisibility(ESlateVisibility::Collapsed);
        }
        if (IngredientNameText)
        {
            IngredientNameText->SetText(FText::FromName(IngredientTag.GetTagName()));
        }
    }

    if (QuantityRatioText)
    {
       QuantityRatioText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), OwnedCount, RequiredCount)));
       
       const bool bHasEnough = (OwnedCount >= RequiredCount);
       QuantityRatioText->SetColorAndOpacity(bHasEnough ? FSlateColor(FLinearColor::White) : FSlateColor(FLinearColor::Red));
    }
}