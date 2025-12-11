#include "UI/Crafting/CraftingIngredientLineEntry.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Crafting/CraftingRecipeManager.h"
#include "Item/ItemTemplate.h"
#include "Styling/SlateColor.h"

void UCraftingIngredientLineEntry::SetData(UCraftingRecipeManager* RecipeManager, FGameplayTag IngredientTag, int32 InOwned, int32 InRequired)
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
                UE_LOG(LogTemp, Log, TEXT("SetData for %s: IngredientIcon pointer is VALID."), *ItemTemplateCDO->DisplayName.ToString());
                IngredientIcon->SetBrushFromTexture(ItemTemplateCDO->IconTexture, true);
                IngredientIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("SetData FAILED: IngredientIcon pointer is NULL!"));
            }

            if (IngredientName_Text)
            {
                UE_LOG(LogTemp, Log, TEXT("SetData for %s: IngredientName_Text pointer is VALID."), *ItemTemplateCDO->DisplayName.ToString());
                IngredientName_Text->SetText(ItemTemplateCDO->DisplayName);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("SetData FAILED: IngredientName_Text pointer is NULL!"));
            }
        }
    }
    else
    {
        if (IngredientName_Text)
        {
            IngredientName_Text->SetText(FText::FromName(IngredientTag.GetTagName()));
        }
    }

    if (Quantity_Text)
    {
        FString QuantityString = FString::Printf(TEXT("%d / %d"), InOwned, InRequired);
        Quantity_Text->SetText(FText::FromString(QuantityString));
        const FLinearColor ColorToSet = (InOwned >= InRequired) ? FLinearColor::White : FLinearColor::Red;
        Quantity_Text->SetColorAndOpacity(FSlateColor(ColorToSet));
    }
}