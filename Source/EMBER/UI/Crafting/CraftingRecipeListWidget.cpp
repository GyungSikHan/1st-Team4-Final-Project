#include "UI/Crafting/CraftingRecipeListWidget.h"
#include "Components/ListView.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Crafting/CraftingRecipeManager.h"
#include "UI/Crafting/CraftingRecipeListItemData.h"
#include "Engine/DataTable.h"

UCraftingRecipeListWidget::UCraftingRecipeListWidget(const FObjectInitializer& ObjectInitializer) 
    : Super(ObjectInitializer)
{
    StationNameText_InList = nullptr;
    RecipeListView = nullptr;
    RecipeListScrollBox = nullptr;
}

void UCraftingRecipeListWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (RecipeListView && !RecipeListView->OnItemClicked().IsBound())
    {
        RecipeListView->OnItemClicked().AddUObject(this, &UCraftingRecipeListWidget::HandleRecipeClicked);
    }
}

void UCraftingRecipeListWidget::SetRecipeList(const TArray<FNamedCraftingRecipe>& InNamedRecipes)
{
    if (!RecipeListView)
    {
        return;
    }
    
    RecipeListView->ClearListItems();

    for (const FNamedCraftingRecipe& NamedRecipe : InNamedRecipes)
    {
       UCraftingRecipeListItemData* NewListItemData = NewObject<UCraftingRecipeListItemData>(this);
       if (NewListItemData)
       {
          NewListItemData->RecipeRowName = NamedRecipe.RecipeRowName;
          NewListItemData->RecipeData = NamedRecipe.RecipeData;
          RecipeListView->AddItem(NewListItemData);
       }
    }
}

void UCraftingRecipeListWidget::SetStationTitle(EStationType InStationType)
{
    if (StationNameText_InList)
    {
        FText TitleToShow = FText::GetEmpty();
        switch (InStationType)
        {
            case EStationType::CraftingTable:
                TitleToShow = FText::FromString(TEXT("제작대"));
                break;
            case EStationType::Furnace:
                TitleToShow = FText::FromString(TEXT("화로"));
                break;
            case EStationType::WeaponTable:
                TitleToShow = FText::FromString(TEXT("무기 제작대"));
                break;
            case EStationType::ClothingTable:
                TitleToShow = FText::FromString(TEXT("의류 제작대"));
                break;
            case EStationType::Campfire:
                TitleToShow = FText::FromString(TEXT("모닥불"));
                break;
            case EStationType::None: 
                TitleToShow = FText::FromString(TEXT("맨손 제작"));
                break;
            default:
                FString StationEnumString = UEnum::GetValueAsString(InStationType);
                FString RawStationName = StationEnumString.RightChop(StationEnumString.Find(TEXT("::")) + 2);
                TitleToShow = FText::FromString(RawStationName);
                break;
        }
        StationNameText_InList->SetText(TitleToShow);
    }
}

void UCraftingRecipeListWidget::HandleRecipeClicked(UObject* ItemObject)
{
    UCraftingRecipeListItemData* ClickedListItemData = Cast<UCraftingRecipeListItemData>(ItemObject);
    if (ClickedListItemData)
    {
        OnRecipeListItemSelected.Broadcast(ClickedListItemData);
    }
}