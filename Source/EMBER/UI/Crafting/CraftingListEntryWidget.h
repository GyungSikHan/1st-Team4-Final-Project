#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UI/Crafting/CraftingRecipeListItemData.h"

#include "CraftingListEntryWidget.generated.h"

class UTextBlock;
class UBorder;
class UImage;
class UItemTemplate;
class UTexture2D;
struct FCraftingRecipeRow;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCraftingListEntryClicked, UCraftingRecipeListItemData*, ListItemData);

UCLASS()
class EMBER_API UCraftingListEntryWidget : public UUserWidget, public IUserObjectListEntry
{
    GENERATED_BODY()

public:
    UCraftingListEntryWidget(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UTextBlock> RecipeNameText;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UImage> RecipeIcon;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
    TObjectPtr<UBorder> SelectionHighlightBorder;

    UPROPERTY(BlueprintAssignable, Category = "ListEntry")
    FOnCraftingListEntryClicked OnThisEntryClicked;

protected:
    virtual void NativeConstruct() override;
    virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override; 
    
    UFUNCTION(BlueprintImplementableEvent, Category = "ListEntry", meta = (DisplayName = "OnUpdateDisplay_BP"))
    void K2_OnUpdateDisplay(const FText& InRecipeName, UTexture2D* InIconTexture, bool bIsCraftable, bool bIsSelected);
    
    UFUNCTION(BlueprintCallable)
    void HandleClick();

    UPROPERTY(BlueprintReadOnly, Category="ListEntry")
    TObjectPtr<UCraftingRecipeListItemData> CurrentListItemDataInternal;

private:
    void UpdateBasicVisuals(bool bIsNowSelected);
    bool CalculateCraftableStatus() const;
    UTexture2D* GetIconTextureFromRecipeData() const;
};