#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFlag.h"
#include "CraftedItemSlotEntryWidget.generated.h"

class UImage;
class UTextBlock;
class UItemTemplate;
class USizeBox;

UCLASS()
class EMBER_API UCraftedItemSlotEntryWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UCraftedItemSlotEntryWidget(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable, Category = "CraftedItemSlot")
    void SetSlotData(TSubclassOf<UItemTemplate> InItemTemplateClass, EItemRarity InRarity, int32 InQuantity);

protected:
    UPROPERTY(BlueprintReadOnly, Category = "CraftedItemSlot", meta = (BindWidgetOptional))
    TObjectPtr<USizeBox> SizeBox_Root;

    UPROPERTY(BlueprintReadOnly, Category = "CraftedItemSlot", meta = (BindWidgetOptional))
    TObjectPtr<UImage> ItemIcon;

    UPROPERTY(BlueprintReadOnly, Category = "CraftedItemSlot", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UItemTemplate> CachedItemTemplateClass;

    UPROPERTY(BlueprintReadOnly, Category = "CraftedItemSlot", meta = (AllowPrivateAccess = "true"))
    EItemRarity CachedRarity;

    UPROPERTY(BlueprintReadOnly, Category = "CraftedItemSlot", meta = (AllowPrivateAccess = "true"))
    int32 CachedQuantity;
    UPROPERTY(BlueprintReadOnly, Category = "CraftedItemSlot", meta = (BindWidgetOptional))
    TObjectPtr<UTextBlock> QuantityText;

    UFUNCTION(BlueprintImplementableEvent, Category = "CraftedItemSlot", meta = (DisplayName = "OnDisplayDataUpdated_BP"))
    void K2_OnDisplayDataUpdated();

private:
    void UpdateWidgetAppearance();


};
