#include "Item/Crafting/CraftingBuilding.h"
#include "Item/Managers/InventoryManagerComponent.h"
#include "Net/UnrealNetwork.h"
#include "Player/EmberPlayerCharacter.h"
#include "Crafting/CraftingSystem.h"
#include "Crafting/CraftingRecipeManager.h"
#include "Item/ItemTemplate.h"

ACraftingBuilding::ACraftingBuilding()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    OutputInventoryComponent = CreateDefaultSubobject<UInventoryManagerComponent>(TEXT("OutputInventory"));
    if (OutputInventoryComponent)
    {
        OutputInventoryComponent->SetIsReplicated(true);
    }
}

void ACraftingBuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ACraftingBuilding, OutputInventoryComponent); 
}

void ACraftingBuilding::Server_ExecuteCrafting_Implementation(FName RecipeRowName, const TArray<FGameplayTag>& SelectedMainIngredientTags, const TArray<int32>& SelectedMainIngredientQuantities, AEmberPlayerCharacter* RequestingPlayer)
{
    if (!HasAuthority() || !RequestingPlayer) return;

    UCraftingSystem* CraftingSystem = RequestingPlayer->FindComponentByClass<UCraftingSystem>();
    if (!CraftingSystem || !CraftingSystem->RecipeManager) return;

    const FCraftingRecipeRow* RecipeData = CraftingSystem->RecipeManager->RecipeDataTable->FindRow<FCraftingRecipeRow>(RecipeRowName, TEXT(""));
    if (!RecipeData) return;

    TMap<FGameplayTag, int32> SelectedMainIngredientsMap;
    for (int32 i = 0; i < SelectedMainIngredientTags.Num(); ++i)
    {
        if (SelectedMainIngredientQuantities.IsValidIndex(i))
            SelectedMainIngredientsMap.Add(SelectedMainIngredientTags[i], SelectedMainIngredientQuantities[i]);
    }

    if (CraftingSystem->HasRequiredMaterials(RequestingPlayer, *RecipeData, SelectedMainIngredientsMap))
    {
        CraftingSystem->ConsumeMaterials_Server_Unsafe(RequestingPlayer, *RecipeData, SelectedMainIngredientsMap);
        
        EItemRarity FinalRarity = CraftingSystem->EvaluateItemRarity(*RecipeData, SelectedMainIngredientsMap);
        if (OutputInventoryComponent)
        {
            OutputInventoryComponent->TryAddItemByRarity(RecipeData->ItemTemplateClass, FinalRarity, 1);
        }
    }
}