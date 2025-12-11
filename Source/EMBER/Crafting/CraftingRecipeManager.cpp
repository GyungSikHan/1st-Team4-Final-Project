#include "Crafting/CraftingRecipeManager.h"
#include "Item/ItemTemplate.h"
#include "System/EmberAssetManager.h"
#include "UI/Data/EmberItemData.h"

const FCraftingRecipeRow* UCraftingRecipeManager::GetRecipeRowByOutputItemID(int32 TemplateID) const
{
    if (!RecipeDataTable || TemplateID == INDEX_NONE)
    {
        return nullptr;
    }

    TArray<FName> RowNames = RecipeDataTable->GetRowNames();
    for (const FName& RowName : RowNames)
    {
        FCraftingRecipeRow* Row = RecipeDataTable->FindRow<FCraftingRecipeRow>(RowName, TEXT(""));
        if (Row && Row->ItemTemplateClass)
        {
            int32 OutputIDFromClass = UEmberItemData::Get().FindItemTemplateIDByClass(Row->ItemTemplateClass);
            if (OutputIDFromClass != INDEX_NONE && OutputIDFromClass == TemplateID)
            {
                return Row;
            }
        }
    }
    return nullptr;
}

void UCraftingRecipeManager::GetAllRecipeRows(TArray<FCraftingRecipeRow*>& OutRows) const
{
    OutRows.Empty();
    if (!RecipeDataTable)
    {
        return;
    }
    
    FString ContextString; 
    RecipeDataTable->GetAllRows(ContextString, OutRows);
}

FGameplayTagContainer UCraftingRecipeManager::GetMaterialTagsForItem(int32 ItemTemplateID) const
{
    const FGameplayTagContainer* FoundTagContainer = ItemIDToMaterialTagMap.Find(ItemTemplateID);
    if (FoundTagContainer)
    {
        return *FoundTagContainer;
    }
    return FGameplayTagContainer();
}

TSubclassOf<UItemTemplate> UCraftingRecipeManager::GetRepresentativeItemForTag(const FGameplayTag& MaterialTag) const
{
    if (MaterialTag.IsValid())
    {
        if (const TSubclassOf<UItemTemplate>* FoundClassPtr = RepresentativeItemForTag.Find(MaterialTag))
        {
            return *FoundClassPtr;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("GetRepresentativeItemForTag: No representative item found for tag %s in the Recipe Manager Data Asset."), *MaterialTag.ToString());
    return nullptr;
}

void UCraftingRecipeManager::PostLoad()
{
    Super::PostLoad();

    UE_LOG(LogTemp, Warning, TEXT("===== DA_CraftingManager: RepresentativeItemForTag Map Contents ====="));
    for (const auto& Pair : RepresentativeItemForTag)
    {
        const FGameplayTag& Tag = Pair.Key;
        const TSubclassOf<UItemTemplate> TemplateClass = Pair.Value;
        
        FString TemplateName = TemplateClass ? TemplateClass->GetName() : TEXT("!!! NULL VALUE !!!");

        UE_LOG(LogTemp, Warning, TEXT("  Key: [%s]  --->  Value: [%s]"), *Tag.ToString(), *TemplateName);
    }
    UE_LOG(LogTemp, Warning, TEXT("=================================================================="));
}
