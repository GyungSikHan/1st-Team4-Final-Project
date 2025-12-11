#include "UI/Crafting/CraftingIngredientWidget.h"
#include "Components/VerticalBox.h"
#include "Crafting/CraftingRecipeManager.h"

UCraftingIngredientWidget::UCraftingIngredientWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    IngredientDisplayBox = nullptr;
    IngredientLineEntryWidgetClass = nullptr;
}

void UCraftingIngredientWidget::UpdateDisplay(UCraftingRecipeManager* RecipeManager, const FCraftingRecipeRow& ForRecipe, const TMap<FGameplayTag, int32>& PlayerOwnedIngredients, int32 CraftingAmount)
{
    if (!IngredientDisplayBox || !IngredientLineEntryWidgetClass || !RecipeManager)
    {
        return;
    }

    IngredientDisplayBox->ClearChildren();

    for (const auto& IngredientPair : ForRecipe.Ingredients)
    {
        const FGameplayTag& IngredientTag = IngredientPair.Key;
        const int32 RequiredQuantity = IngredientPair.Value * CraftingAmount;

        const int32 OwnedQuantity = PlayerOwnedIngredients.Contains(IngredientTag) ? PlayerOwnedIngredients[IngredientTag] : 0;

        UCraftingIngredientLineEntry* EntryWidget = CreateWidget<UCraftingIngredientLineEntry>(this, IngredientLineEntryWidgetClass);
        if (EntryWidget)
        {
            EntryWidget->SetData(RecipeManager, IngredientTag, OwnedQuantity, RequiredQuantity);
            IngredientDisplayBox->AddChildToVerticalBox(EntryWidget);
        }
    }
}