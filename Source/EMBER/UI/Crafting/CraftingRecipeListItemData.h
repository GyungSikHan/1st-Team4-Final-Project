#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Crafting/CraftingRecipeManager.h" 
#include "CraftingRecipeListItemData.generated.h"

UCLASS(BlueprintType)
class EMBER_API UCraftingRecipeListItemData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Recipe Data")
	FCraftingRecipeRow RecipeData;

	UPROPERTY(BlueprintReadOnly, Category = "Recipe Data")
	FName RecipeRowName; 
};