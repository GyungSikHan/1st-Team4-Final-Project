#include "UI/Crafting/CraftingOutputBoxWidget.h"
#include "UI/Crafting/CraftedItemSlotEntryWidget.h"
#include "Item/ItemInstance.h"
#include "Item/ItemTemplate.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/TextBlock.h"      

UCraftingOutputBoxWidget::UCraftingOutputBoxWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    NumRows = 1;
    NumColumns = 5; 
    CraftedItemSlotEntryClass = nullptr;
    OutputSlotsGridPanel = nullptr;
    TitleTextWidget = nullptr;
}

void UCraftingOutputBoxWidget::NativeConstruct()
{
    Super::NativeConstruct();
    RefreshSlotsUI();
}

const UItemTemplate* UCraftingOutputBoxWidget::GetTemplateFromClass(TSubclassOf<UItemTemplate> InClass) const
{
    if (InClass)
    {
        return InClass->GetDefaultObject<UItemTemplate>();
    }
    return nullptr;
}

void UCraftingOutputBoxWidget::RefreshSlotsUI()
{
    UE_LOG(LogTemp, Warning, TEXT("=== RefreshSlotsUI - 현재 슬롯 상태 ==="));
    for (int32 i = 0; i < OutputSlotEntries.Num(); ++i)
    {
        const auto& TestSlot = OutputSlotEntries[i];
        UE_LOG(LogTemp, Warning, TEXT("Slot %d: Class=%s / Qty=%d"),
            i,
            TestSlot.ItemTemplateClass ? *TestSlot.ItemTemplateClass->GetName() : TEXT("NULL"),
            TestSlot.Quantity);
    }
    if (!OutputSlotsGridPanel || !CraftedItemSlotEntryClass)
    {
        UE_LOG(LogTemp, Error, TEXT("OutputSlotsGridPanel or CraftedItemSlotEntryClass is NULL."));
        return;
    }

    const int32 CurrentMaxSlots = GetMaxSlots();

    // Create widgets if not already created
    while (OutputSlotEntries.Num() < CurrentMaxSlots)
    {
        OutputSlotEntries.Add(FCraftingOutputSlotEntry());
    }

    while (OutputSlotWidgets.Num() < CurrentMaxSlots)
    {
        UCraftedItemSlotEntryWidget* EntryWidget = CreateWidget<UCraftedItemSlotEntryWidget>(this, CraftedItemSlotEntryClass);
        OutputSlotWidgets.Add(EntryWidget);

        const int32 Index = OutputSlotWidgets.Num() - 1;
        const int32 TargetRow = Index / NumColumns;
        const int32 TargetColumn = Index % NumColumns;

        UUniformGridSlot* GridSlot = OutputSlotsGridPanel->AddChildToUniformGrid(EntryWidget, TargetRow, TargetColumn);
        if (GridSlot)
        {
            GridSlot->SetHorizontalAlignment(HAlign_Fill);
            GridSlot->SetVerticalAlignment(VAlign_Fill);
        }
    }

    // Update all widgets with current slot data
    for (int32 i = 0; i < OutputSlotWidgets.Num(); ++i)
    {
        if (OutputSlotWidgets[i])
        {
            const FCraftingOutputSlotEntry& SlotEntry = OutputSlotEntries[i];
            UE_LOG(LogTemp, Warning, TEXT("[UI] -> SetSlotData(%d) - Class=%s / Rarity=%d / Qty=%d"),
                i,
                SlotEntry.ItemTemplateClass ? *SlotEntry.ItemTemplateClass->GetName() : TEXT("NULL"),
                static_cast<uint8>(SlotEntry.Rarity),
                SlotEntry.Quantity);
            OutputSlotWidgets[i]->SetSlotData(SlotEntry.ItemTemplateClass, SlotEntry.Rarity, SlotEntry.Quantity);
        }
    }

    OnContentsChanged.Broadcast();
}


bool UCraftingOutputBoxWidget::TryAddItem(TSubclassOf<UItemTemplate> InItemTemplateClass, EItemRarity InRarity, int32 InQuantity)
{
    if (!InItemTemplateClass || InQuantity <= 0) return false;

    const UItemTemplate* TemplateOfItemToAdd = InItemTemplateClass->GetDefaultObject<UItemTemplate>();
    if (!TemplateOfItemToAdd) return false;

    const int32 MaxStackSize = TemplateOfItemToAdd->MaxStackCount > 0 ? TemplateOfItemToAdd->MaxStackCount : 1;
    int32 RemainingQuantity = InQuantity;
    UE_LOG(LogTemp, Warning, TEXT("PRE-UI ItemTemplateClass: %s, Qty: %d"),
        *InItemTemplateClass->GetName(),
        InQuantity);
    // ✅ 아이템 스택 추가 (기존과 동일)
    for (int32 i = 0; i < OutputSlotEntries.Num(); ++i)
    {
        const auto& TestSlot = OutputSlotEntries[i];
        UE_LOG(LogTemp, Warning, TEXT("  Slot %d - Class: %s / Qty: %d"),
            i,
            TestSlot.ItemTemplateClass ? *TestSlot.ItemTemplateClass->GetName() : TEXT("NULL"),
            TestSlot.Quantity);
        FCraftingOutputSlotEntry& SlotEntry = OutputSlotEntries[i];
        if (SlotEntry.ItemTemplateClass == InItemTemplateClass && SlotEntry.Rarity == InRarity && SlotEntry.Quantity < MaxStackSize)
        {
            const int32 CanAddAmount = FMath::Min(RemainingQuantity, MaxStackSize - SlotEntry.Quantity);
            SlotEntry.Quantity += CanAddAmount;
            RemainingQuantity -= CanAddAmount;
            if (RemainingQuantity <= 0) break;
        }
    }

    // ✅ 빈 슬롯에 새로 삽입
    if (RemainingQuantity > 0)
    {
        for (int32 i = 0; i < OutputSlotEntries.Num(); ++i)
        {
            FCraftingOutputSlotEntry& SlotEntry = OutputSlotEntries[i];
            if (!SlotEntry.ItemTemplateClass || SlotEntry.Quantity <= 0)
            {
                SlotEntry.ItemTemplateClass = InItemTemplateClass;
                SlotEntry.Rarity = InRarity;
                SlotEntry.Quantity = FMath::Min(RemainingQuantity, MaxStackSize);
                RemainingQuantity -= SlotEntry.Quantity;

                if (RemainingQuantity <= 0) break;
            }
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("=== OutputSlotEntries 상태 (TryAddItem 이후) ==="));
    for (int32 i = 0; i < OutputSlotEntries.Num(); ++i)
    {
        const auto& TestSlot = OutputSlotEntries[i];
        UE_LOG(LogTemp, Warning, TEXT("Slot %d: Class=%s / Rarity=%d / Qty=%d"),
            i,
            TestSlot.ItemTemplateClass ? *TestSlot.ItemTemplateClass->GetName() : TEXT("NULL"),
            static_cast<uint8>(TestSlot.Rarity),
            TestSlot.Quantity);
    }
    // ✅ 슬롯 데이터 반영된 뒤 UI 갱신
    RefreshSlotsUI();

    return (RemainingQuantity <= 0);
}


TSubclassOf<UItemTemplate> UCraftingOutputBoxWidget::GetItemTemplateClassFromSlot(int32 FlatSlotIndex, EItemRarity& OutRarity, int32& OutQuantity)
{
    OutRarity = EItemRarity::Common;
    OutQuantity = 0;

    if (OutputSlotEntries.IsValidIndex(FlatSlotIndex))
    {
        const FCraftingOutputSlotEntry& SlotEntry = OutputSlotEntries[FlatSlotIndex];
        OutRarity = SlotEntry.Rarity;
        OutQuantity = SlotEntry.Quantity;
        return SlotEntry.ItemTemplateClass;
    }
    return nullptr;
}

void UCraftingOutputBoxWidget::ClearSlot(int32 FlatSlotIndex)
{
    if (OutputSlotEntries.IsValidIndex(FlatSlotIndex))
    {
        OutputSlotEntries[FlatSlotIndex] = FCraftingOutputSlotEntry();
        RefreshSlotsUI();
    }
}

bool UCraftingOutputBoxWidget::IsFull() const
{
    for (const FCraftingOutputSlotEntry& SlotEntry : OutputSlotEntries)
    {
        if (!SlotEntry.ItemTemplateClass) return false; 
        
        const UItemTemplate* Template = GetTemplateFromClass(SlotEntry.ItemTemplateClass);
        if (!Template) continue;
        
        const int32 MaxStack = Template->MaxStackCount > 0 ? Template->MaxStackCount : 1;
        if (SlotEntry.Quantity < MaxStack)
        {
            return false;
        }
    }
    return true; 
}

int32 UCraftingOutputBoxWidget::GetMaxSlots() const
{
    return NumRows * NumColumns;
}

void UCraftingOutputBoxWidget::SetGridDimensions(int32 InNumRows, int32 InNumColumns)
{
    NumRows = FMath::Max(1, InNumRows);
    NumColumns = FMath::Max(1, InNumColumns);
    RefreshSlotsUI();
}