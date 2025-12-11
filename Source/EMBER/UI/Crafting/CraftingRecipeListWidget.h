#pragma once

#include "CoreMinimal.h"
#include "UI/Common/EmberActivatableWidget.h"
#include "Crafting/CraftingRecipeManager.h" 
#include "UI/Crafting/CraftingRecipeListItemData.h" 
#include "CraftingRecipeListWidget.generated.h"

class UListView;
class UScrollBox;
class UTextBlock;
struct FCraftingRecipeRow; 
class UCraftingRecipeListItemData; 

USTRUCT(BlueprintType)
struct FNamedCraftingRecipe
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Recipe")
    FName RecipeRowName;

    UPROPERTY(BlueprintReadOnly, Category = "Recipe")
    FCraftingRecipeRow RecipeData;

    FNamedCraftingRecipe() : RecipeRowName(NAME_None) {}

    FNamedCraftingRecipe(const FName& InName, const FCraftingRecipeRow& InData)
        : RecipeRowName(InName), RecipeData(InData)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecipeListItemSelectedSignature, UCraftingRecipeListItemData*, SelectedItemData);

UCLASS()
class EMBER_API UCraftingRecipeListWidget : public UEmberActivatableWidget
{
    GENERATED_BODY()

public:
    UCraftingRecipeListWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, Category = "Crafting")
    void SetRecipeList(const TArray<FNamedCraftingRecipe>& InNamedRecipes);

    UFUNCTION(BlueprintCallable, Category = "Crafting")
    void SetStationTitle(EStationType InStationType);
    
    UPROPERTY(BlueprintAssignable, Category = "Crafting")
    FOnRecipeListItemSelectedSignature OnRecipeListItemSelected;

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(BlueprintReadOnly, Category = "RecipeList", meta = (BindWidgetOptional))
    TObjectPtr<UTextBlock> StationNameText_InList; 

    UPROPERTY(BlueprintReadOnly, Category = "RecipeList", meta = (BindWidget))
    TObjectPtr<UListView> RecipeListView;

    UPROPERTY(BlueprintReadOnly, Category = "RecipeList", meta = (BindWidgetOptional))
    TObjectPtr<UScrollBox> RecipeListScrollBox;

    UFUNCTION()
    void HandleRecipeClicked(UObject* ItemObject);
};