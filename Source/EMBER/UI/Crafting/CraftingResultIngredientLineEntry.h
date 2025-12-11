#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "CraftingResultIngredientLineEntry.generated.h"

class UTextBlock;
class UHorizontalBox;
class UImage;
class UCraftingRecipeManager;

UCLASS()
class EMBER_API UCraftingResultIngredientLineEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	UCraftingResultIngredientLineEntry(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Ingredient Line Data")
	void SetLineData(UCraftingRecipeManager* RecipeManager, FGameplayTag IngredientTag, int32 OwnedCount, int32 RequiredCount);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Ingredient Line Data", meta = (BindWidgetOptional))
	TObjectPtr<UImage> IngredientIcon;

	UPROPERTY(BlueprintReadOnly, Category = "Ingredient Line Data", meta = (BindWidget))
	TObjectPtr<UTextBlock> IngredientNameText;

	UPROPERTY(BlueprintReadOnly, Category = "Ingredient Line Data", meta = (BindWidget))
	TObjectPtr<UTextBlock> QuantityRatioText;
    
	UPROPERTY(BlueprintReadOnly, Category = "Ingredient Line Data", meta = (BindWidgetOptional))
	TObjectPtr<UHorizontalBox> LayoutHorizontalBox;
};