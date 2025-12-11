#pragma once

#include "CoreMinimal.h"
#include "UI/Common/EmberActivatableWidget.h"
#include "GameFlag.h"
#include "CraftingOutputBoxWidget.generated.h"

class UUniformGridPanel;
class UCraftedItemSlotEntryWidget;
class UItemTemplate;
class UTextBlock;

USTRUCT(BlueprintType)
struct FCraftingOutputSlotEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "OutputSlot")
    TSubclassOf<UItemTemplate> ItemTemplateClass;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "OutputSlot")
    EItemRarity Rarity;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "OutputSlot")
    int32 Quantity;

    FCraftingOutputSlotEntry() : ItemTemplateClass(nullptr), Rarity(EItemRarity::Common), Quantity(0) {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOutputBoxContentsChanged);

UCLASS()
class EMBER_API UCraftingOutputBoxWidget : public UEmberActivatableWidget
{
    GENERATED_BODY()

public:
    UCraftingOutputBoxWidget(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable, Category = "CraftingOutput")
    bool TryAddItem(TSubclassOf<UItemTemplate> InItemTemplateClass, EItemRarity InRarity, int32 InQuantity);

    UFUNCTION(BlueprintCallable, Category = "CraftingOutput")
    TSubclassOf<UItemTemplate> GetItemTemplateClassFromSlot(int32 FlatSlotIndex, EItemRarity& OutRarity, int32& OutQuantity);
    
    UFUNCTION(BlueprintCallable, Category = "CraftingOutput")
    void ClearSlot(int32 FlatSlotIndex);

    UFUNCTION(BlueprintPure, Category = "CraftingOutput")
    bool IsFull() const;

    UFUNCTION(BlueprintPure, Category = "CraftingOutput")
    int32 GetMaxSlots() const;

    UFUNCTION(BlueprintCallable, Category = "Layout")
    void SetGridDimensions(int32 InNumRows, int32 InNumColumns);

    UPROPERTY(BlueprintAssignable, Category = "CraftingOutput")
    FOnOutputBoxContentsChanged OnContentsChanged;

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CraftingOutput|Layout", meta = (ClampMin = "1", UIMin = "1"))
    int32 NumRows;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CraftingOutput|Layout", meta = (ClampMin = "1", UIMin = "1"))
    int32 NumColumns;

    UPROPERTY(BlueprintReadOnly, Category = "CraftingOutput")
    TArray<FCraftingOutputSlotEntry> OutputSlotEntries;

    UPROPERTY(EditDefaultsOnly, Category = "CraftingOutput")
    TSubclassOf<UCraftedItemSlotEntryWidget> CraftedItemSlotEntryClass;

    UPROPERTY(BlueprintReadOnly, Category = "CraftingOutput", meta = (BindWidgetOptional))
    TObjectPtr<UUniformGridPanel> OutputSlotsGridPanel;

    UPROPERTY(BlueprintReadOnly, Category = "CraftingOutput", meta = (BindWidgetOptional))
    TObjectPtr<UTextBlock> TitleTextWidget;

    UPROPERTY()
    TArray<UCraftedItemSlotEntryWidget*> OutputSlotWidgets;

private:
    void RefreshSlotsUI();
    const UItemTemplate* GetTemplateFromClass(TSubclassOf<UItemTemplate> InClass) const;
};