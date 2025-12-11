#include "CraftingMainMaterialWidget.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"

UCraftingMainMaterialWidget::UCraftingMainMaterialWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , RequiredMainMaterialTotalCount(0)
{
}

void UCraftingMainMaterialWidget::InitializeSelector(const TArray<FSelectableMainMaterialInfo>& AvailableMaterialsFromInventory, int32 InRequiredTotalCount)
{
    ClearStagedMaterials();
    DisplayableMaterials = AvailableMaterialsFromInventory;
    RequiredMainMaterialTotalCount = InRequiredTotalCount;

    if (MaterialListView)
    {
        MaterialListView->ClearListItems();
        for (const FSelectableMainMaterialInfo& Info : DisplayableMaterials)
        {
            USelectableMaterialEntryData* EntryDataObject = NewObject<USelectableMaterialEntryData>(this);
            if(EntryDataObject)
            {
                EntryDataObject->MaterialInfo = Info;
                MaterialListView->AddItem(EntryDataObject);
            }
        }
    }
    UpdateStagedCountText();
    BroadcastSelectionChange();
}

bool UCraftingMainMaterialWidget::TryStageMaterial(FGameplayTag MaterialTag, int32 QuantityToStage)
{
    if (QuantityToStage <= 0) return false;

    int32 CurrentOverallStagedCount = GetCurrentStagedTotalCount();
    if (CurrentOverallStagedCount + QuantityToStage > RequiredMainMaterialTotalCount)
    {
        return false;
    }

    FSelectableMainMaterialInfo* FoundDisplayInfo = DisplayableMaterials.FindByPredicate(
        [&](const FSelectableMainMaterialInfo& Info){ return Info.MaterialTag == MaterialTag; });

    if (!FoundDisplayInfo) return false;

    int32 CurrentStagedForThisTag = StagedMainIngredients.FindRef(MaterialTag);
    if (CurrentStagedForThisTag + QuantityToStage > FoundDisplayInfo->AvailableCountInInventory)
    {
        return false;
    }

    StagedMainIngredients.FindOrAdd(MaterialTag) += QuantityToStage;
    if(FoundDisplayInfo) FoundDisplayInfo->CurrentlyStagedCount = StagedMainIngredients.FindRef(MaterialTag);

    UpdateStagedCountText();
    BroadcastSelectionChange();
    return true;
}

bool UCraftingMainMaterialWidget::TryUnstageMaterial(FGameplayTag MaterialTag, int32 QuantityToUnstage)
{
    if (QuantityToUnstage <= 0) return false;

    int32* FoundStagedCountPtr = StagedMainIngredients.Find(MaterialTag);
    if (!FoundStagedCountPtr || *FoundStagedCountPtr == 0)
    {
        return false;
    }

    int32 AmountToActuallyUnstage = FMath::Min(QuantityToUnstage, *FoundStagedCountPtr);
    *FoundStagedCountPtr -= AmountToActuallyUnstage;

    if (*FoundStagedCountPtr <= 0)
    {
        StagedMainIngredients.Remove(MaterialTag);
    }

    FSelectableMainMaterialInfo* FoundDisplayInfo = DisplayableMaterials.FindByPredicate(
        [&](const FSelectableMainMaterialInfo& Info){ return Info.MaterialTag == MaterialTag; });
    if(FoundDisplayInfo)
    {
         FoundDisplayInfo->CurrentlyStagedCount = StagedMainIngredients.FindRef(MaterialTag);
    }

    UpdateStagedCountText();
    BroadcastSelectionChange();
    return true;
}

void UCraftingMainMaterialWidget::ClearStagedMaterials()
{
    StagedMainIngredients.Empty();
    for(FSelectableMainMaterialInfo& Info : DisplayableMaterials)
    {
        Info.CurrentlyStagedCount = 0;
    }
    UpdateStagedCountText();
    BroadcastSelectionChange();
}

const TMap<FGameplayTag, int32>& UCraftingMainMaterialWidget::GetStagedMainIngredientsMap() const
{
    return StagedMainIngredients;
}

int32 UCraftingMainMaterialWidget::GetCurrentStagedTotalCount() const
{
    int32 TotalCount = 0;
    for (const auto& Pair : StagedMainIngredients)
    {
        TotalCount += Pair.Value;
    }
    return TotalCount;
}

void UCraftingMainMaterialWidget::UpdateStagedCountText()
{
    if (StagedCountText)
    {
        FText Text = FText::Format(FText::FromString(TEXT("선택된 주재료: {0} / {1}")),
                                   FText::AsNumber(GetCurrentStagedTotalCount()),
                                   FText::AsNumber(RequiredMainMaterialTotalCount));
        StagedCountText->SetText(Text);
    }
}

void UCraftingMainMaterialWidget::BroadcastSelectionChange()
{
    FSelectedIngredientsMapWrapper WrapperToSend(StagedMainIngredients);
    OnSelectionChanged.Broadcast(WrapperToSend);
}