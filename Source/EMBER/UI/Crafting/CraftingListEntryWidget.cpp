#include "UI/Crafting/CraftingListEntryWidget.h"
#include "Item/ItemTemplate.h" 
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "UI/Crafting/CraftingRecipeListItemData.h" 
#include "Crafting/CraftingRecipeManager.h" 
#include "Crafting/CraftingSystem.h" 
#include "Player/EmberPlayerCharacter.h" 
#include "UI/Data/EmberItemData.h"
#include "Engine/Texture2D.h"

UCraftingListEntryWidget::UCraftingListEntryWidget(const FObjectInitializer& ObjectInitializer) 
    : Super(ObjectInitializer)
{
    CurrentListItemDataInternal = nullptr;
    SelectionHighlightBorder = nullptr;
    RecipeNameText = nullptr;
    RecipeIcon = nullptr;
}

void UCraftingListEntryWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

bool UCraftingListEntryWidget::CalculateCraftableStatus() const
{
    if (!CurrentListItemDataInternal) return false;

    AEmberPlayerCharacter* Player = Cast<AEmberPlayerCharacter>(GetOwningPlayerPawn());
    UCraftingSystem* CraftingSystem = Player ? Player->FindComponentByClass<UCraftingSystem>() : nullptr;
    
    if (CraftingSystem && Player)
    {
       TMap<FGameplayTag, int32> EmptySelectedMainIngredients;
       return CraftingSystem->HasRequiredMaterials(Player, CurrentListItemDataInternal->RecipeData, EmptySelectedMainIngredients);
    }
    return false;
}

UTexture2D* UCraftingListEntryWidget::GetIconTextureFromRecipeData() const
{
    if (CurrentListItemDataInternal && CurrentListItemDataInternal->RecipeData.ItemTemplateClass)
    {
        const UItemTemplate* ItemTemplateCDO = CurrentListItemDataInternal->RecipeData.ItemTemplateClass->GetDefaultObject<UItemTemplate>();
        if (ItemTemplateCDO)
        {
            return ItemTemplateCDO->IconTexture;
        }
    }
    return nullptr;
}

void UCraftingListEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    CurrentListItemDataInternal = Cast<UCraftingRecipeListItemData>(ListItemObject);

    UpdateBasicVisuals(IsListItemSelected()); 
    
    FText RecipeNameForDisplay = FText::FromString(TEXT("Invalid Recipe"));
    if(CurrentListItemDataInternal) RecipeNameForDisplay = CurrentListItemDataInternal->RecipeData.RecipeDisplayName;
    
    UTexture2D* IconTextureForDisplay = GetIconTextureFromRecipeData();
    bool bCanCraft = CalculateCraftableStatus();
    
    K2_OnUpdateDisplay(RecipeNameForDisplay, IconTextureForDisplay, bCanCraft, IsListItemSelected());
}

void UCraftingListEntryWidget::UpdateBasicVisuals(bool bIsNowSelected)
{
    if (RecipeNameText)
    {
        if(CurrentListItemDataInternal)
        {
            RecipeNameText->SetText(CurrentListItemDataInternal->RecipeData.RecipeDisplayName);
        }
        else
        {
            RecipeNameText->SetText(FText::FromString(TEXT("...")));
        }
    }

    if (SelectionHighlightBorder)
    {
        SelectionHighlightBorder->SetVisibility(bIsNowSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
    }
}

void UCraftingListEntryWidget::HandleClick()
{
    if (CurrentListItemDataInternal) 
    {
       OnThisEntryClicked.Broadcast(CurrentListItemDataInternal);
    }
}