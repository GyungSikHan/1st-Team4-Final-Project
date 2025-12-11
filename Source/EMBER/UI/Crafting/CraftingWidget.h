#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "UI/Common/EmberActivatableWidget.h"
#include "UI/Crafting/CraftingRecipeListWidget.h"
#include "CraftingWidget.generated.h"

class UCraftingRecipeListWidget;
class UCraftingResultWidget;
class UCraftingMainMaterialWidget;
class UCraftingOutputBoxWidget;
class UCraftingIngredientWidget;
class UWidgetSwitcher;
class ACraftingBuilding;

USTRUCT(BlueprintType)
struct FCraftingWidgetInitializeMessage
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ACraftingBuilding> CraftingBuilding;
};


UCLASS()
class EMBER_API UCraftingWidget : public UEmberActivatableWidget
{
	GENERATED_BODY()

public:
	UCraftingWidget(const FObjectInitializer& ObjectInitializer);
	
	void InitializeForStation(ACraftingBuilding* InStationActor, FName OptionalInitialRecipeRowName = NAME_None);
	
protected:
	virtual void NativeConstruct() override;
	
	virtual void NativeDestruct() override;
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(BlueprintReadOnly, Category = "RecipeList", meta = (BindWidget))
	TObjectPtr<UCraftingRecipeListWidget> RecipeListWidget;
	
	UPROPERTY(BlueprintReadOnly, Category = "RecipeList", meta = (BindWidget))
	TObjectPtr<UCraftingResultWidget> SelectedRecipeDisplayWidget;

public:
	void ConstructUI(FGameplayTag Channel, const FCraftingWidgetInitializeMessage& Message);
	void DestructUI();
	
	UPROPERTY(EditAnywhere, meta=(Categories="Message"))
	FGameplayTag MessageChannelTag;

private:
	void PopulateActiveRecipeList();
	void ClearSelectedMainIngredients();
	void UpdateSelectedRecipe(int32 Direction);
	void AdjustCraftAmount(int32 Delta);
	void RefreshAll();
	
	UFUNCTION(BlueprintCallable, Category = "Crafting")
	void AttemptCraftCurrentRecipe();

	UFUNCTION()
	void HandleRecipeSelectedFromList(UCraftingRecipeListItemData* SelectedItemData);
	
	UFUNCTION()
	void HandleMainMaterialSelectionChanged(const FSelectedIngredientsMapWrapper& Wrapper);

	UFUNCTION()
	void HandleCraftRequest();

	TArray<FNamedCraftingRecipe> ActiveRecipeList;
	int32 SelectedRecipeIndex;
	int32 CraftAmount;
	FName SelectedRecipeName;
	TMap<FGameplayTag, int32> CurrentSelectedMainIngredients;
	
	UPROPERTY()
	TObjectPtr<ACraftingBuilding> CurrentStationActorRef;
	
	EStationType CurrentStationTypeForUI;

	FGameplayMessageListenerHandle MessageListenerHandle;
};