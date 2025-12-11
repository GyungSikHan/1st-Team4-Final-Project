#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "CraftingIngredientLineEntry.generated.h"

class UTextBlock;
class UHorizontalBox;
class UImage;
class UCraftingRecipeManager;

UCLASS()
class EMBER_API UCraftingIngredientLineEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "IngredientLine")
	void SetData(UCraftingRecipeManager* RecipeManager, FGameplayTag IngredientTag, int32 InOwned, int32 InRequired);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "IngredientLine", meta = (BindWidgetOptional))
	TObjectPtr<UImage> IngredientIcon;

	UPROPERTY(BlueprintReadOnly, Category = "IngredientLine", meta = (BindWidget))
	TObjectPtr<UTextBlock> IngredientName_Text;

	UPROPERTY(BlueprintReadOnly, Category = "IngredientLine", meta = (BindWidget))
	TObjectPtr<UTextBlock> Quantity_Text;

	UPROPERTY(BlueprintReadOnly, Category = "IngredientLine", meta = (BindWidgetOptional))
	TObjectPtr<UHorizontalBox> LayoutHBox; 
};