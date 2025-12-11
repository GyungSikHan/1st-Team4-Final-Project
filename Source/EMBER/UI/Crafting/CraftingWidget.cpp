#include "UI/Crafting/CraftingWidget.h"
#include "UI/Crafting/CraftingRecipeListWidget.h"
#include "UI/Crafting/CraftingIngredientWidget.h"
#include "UI/Crafting/CraftingResultWidget.h"
#include "UI/Crafting/CraftingMainMaterialWidget.h"
#include "UI/Crafting/CraftingOutputBoxWidget.h"
#include "Crafting/CraftingSystem.h"
#include "Crafting/CraftingRecipeManager.h"
#include "Player/EmberPlayerCharacter.h"
#include "Item/Crafting/CraftingBuilding.h"
#include "Item/ItemInstance.h"
#include "Component/MontageSystemComponent.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Managers/ItemManagerComponent.h"

UCraftingWidget::UCraftingWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    SelectedRecipeIndex = -1;
    CraftAmount = 1;
    SelectedRecipeName = NAME_None;
    CurrentStationActorRef = nullptr;
    CurrentStationTypeForUI = EStationType::None;
}

void UCraftingWidget::NativeConstruct()
{    Super::NativeConstruct();
    if (RecipeListWidget)
    {
        RecipeListWidget->OnRecipeListItemSelected.AddDynamic(this, &UCraftingWidget::HandleRecipeSelectedFromList);
    }
    
    if (SelectedRecipeDisplayWidget)
    {
        SelectedRecipeDisplayWidget->OnCraftRequested.Clear();
        SelectedRecipeDisplayWidget->OnCraftRequested.AddUniqueDynamic(this, &UCraftingWidget::HandleCraftRequest);
    }

    UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
    MessageListenerHandle = MessageSubsystem.RegisterListener(MessageChannelTag, this, &ThisClass::ConstructUI);
}

void UCraftingWidget::NativeDestruct()
{
    DestructUI();
	
    UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
    MessageSubsystem.UnregisterListener(MessageListenerHandle);
    
    Super::NativeDestruct();
}

FReply UCraftingWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    const FKey Key = InKeyEvent.GetKey();
    if (Key == EKeys::E) { AttemptCraftCurrentRecipe(); return FReply::Handled(); }
    
    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UCraftingWidget::InitializeForStation(ACraftingBuilding* InStationActor, FName OptionalInitialRecipeRowName)
{
    CurrentStationActorRef = InStationActor;
    CurrentStationTypeForUI = InStationActor ? InStationActor->StationType : EStationType::None;

    SelectedRecipeIndex = -1;
    SelectedRecipeName = NAME_None;
    
    PopulateActiveRecipeList();

    if (!OptionalInitialRecipeRowName.IsNone())
    {
        for (int32 i = 0; i < ActiveRecipeList.Num(); ++i)
        {
            if (ActiveRecipeList[i].RecipeRowName == OptionalInitialRecipeRowName)
            {
                SelectedRecipeIndex = i;
                SelectedRecipeName = OptionalInitialRecipeRowName;
                break;
            }
        }
    }

    if (ActiveRecipeList.Num() > 0 && SelectedRecipeIndex == -1)
    {
        SelectedRecipeIndex = 0;
        SelectedRecipeName = ActiveRecipeList[0].RecipeRowName;
    }

    ClearSelectedMainIngredients();
    RefreshAll();
}

void UCraftingWidget::ConstructUI(FGameplayTag Channel, const FCraftingWidgetInitializeMessage& Message)
{
    InitializeForStation(Message.CraftingBuilding, NAME_None);
}

void UCraftingWidget::DestructUI()
{
    if (RecipeListWidget)
    {
        RecipeListWidget->OnRecipeListItemSelected.RemoveDynamic(this, &UCraftingWidget::HandleRecipeSelectedFromList);
    }
    
    if (SelectedRecipeDisplayWidget)
    {
        SelectedRecipeDisplayWidget->OnCraftRequested.RemoveDynamic(this, &UCraftingWidget::HandleCraftRequest);
    }
}

void UCraftingWidget::PopulateActiveRecipeList()
{
    ActiveRecipeList.Empty();

    AEmberPlayerCharacter* Player = Cast<AEmberPlayerCharacter>(GetOwningPlayerPawn());
    if (!Player) return;
    
    UCraftingSystem* Sys = Player->FindComponentByClass<UCraftingSystem>();
    if (!Sys || !Sys->RecipeManager) return;
    
    FGameplayTag CurrentStationTag;
    if (CurrentStationActorRef)
    {
        CurrentStationTag = CurrentStationActorRef->GetStationTag();
    }
    
    TArray<FCraftingRecipeRow*> AllRecipeRows;
    Sys->RecipeManager->GetAllRecipeRows(AllRecipeRows);

    if(Sys->RecipeManager->RecipeDataTable)
    {
        TArray<FName> AllRowNames = Sys->RecipeManager->RecipeDataTable->GetRowNames();
        for (int32 i = 0; i < AllRecipeRows.Num(); ++i)
        {
            if (FCraftingRecipeRow* RowPtr = AllRecipeRows[i])
            {
                if (Sys->CanCraftRecipeAtStation(*RowPtr, CurrentStationTag))
                {
                    if (AllRowNames.IsValidIndex(i))
                    {
                        ActiveRecipeList.Add(FNamedCraftingRecipe{ AllRowNames[i], *RowPtr });
                    }
                }
            }
        }
    }
}

void UCraftingWidget::ClearSelectedMainIngredients()
{
    CurrentSelectedMainIngredients.Empty();
}

void UCraftingWidget::UpdateSelectedRecipe(int32 Direction)
{
    if (ActiveRecipeList.Num() == 0) return;

    SelectedRecipeIndex = (SelectedRecipeIndex + Direction + ActiveRecipeList.Num()) % ActiveRecipeList.Num();
    SelectedRecipeName = ActiveRecipeList.IsValidIndex(SelectedRecipeIndex) ? ActiveRecipeList[SelectedRecipeIndex].RecipeRowName : NAME_None;
    
    ClearSelectedMainIngredients();
    RefreshAll();
}

void UCraftingWidget::AdjustCraftAmount(int32 Delta)
{
    CraftAmount = FMath::Clamp(CraftAmount + Delta, 1, 99);
    RefreshAll();
}

void UCraftingWidget::RefreshAll()
{
    const FCraftingRecipeRow* SelectedRecipeDataPtr = nullptr;
    if (ActiveRecipeList.IsValidIndex(SelectedRecipeIndex))
    {
        SelectedRecipeDataPtr = &ActiveRecipeList[SelectedRecipeIndex].RecipeData;
    }

    if (RecipeListWidget)
    {
        RecipeListWidget->SetRecipeList(ActiveRecipeList);
        RecipeListWidget->SetStationTitle(CurrentStationTypeForUI);
    }

    TMap<FGameplayTag,int32> PlayerIngredients;
    AEmberPlayerCharacter* Player = Cast<AEmberPlayerCharacter>(GetOwningPlayerPawn());
    UCraftingSystem* CraftingSystem = Player ? Player->FindComponentByClass<UCraftingSystem>() : nullptr;
    if (Player && CraftingSystem)
    {
        PlayerIngredients = CraftingSystem->AggregateIngredients(Player);
    }

    if (SelectedRecipeDisplayWidget)
    {
        if (SelectedRecipeDataPtr && CraftingSystem && CraftingSystem->RecipeManager)
        {
            SelectedRecipeDisplayWidget->SetRecipeDetails(CraftingSystem->RecipeManager, *SelectedRecipeDataPtr, PlayerIngredients, CraftAmount);
        }
        else
        {
            SelectedRecipeDisplayWidget->ClearDetails();
        }
    }
}

void UCraftingWidget::AttemptCraftCurrentRecipe()
{
    if (!ActiveRecipeList.IsValidIndex(SelectedRecipeIndex)) return;
    
    AEmberPlayerCharacter* Player = Cast<AEmberPlayerCharacter>(GetOwningPlayerPawn());
    UCraftingSystem* Sys = Player ? Player->FindComponentByClass<UCraftingSystem>() : nullptr;
    UItemManagerComponent* ItemManagerComponent = Player->FindComponentByClass<UItemManagerComponent>();
    if (!Sys || !Player || !ItemManagerComponent) return;
    
    if (CurrentStationActorRef)
    {
        if (UAnimMontage* StationAnimation = CurrentStationActorRef->GetCraftingAnimation())
        {
            if (UMontageSystemComponent* MontageComp = Player->FindComponentByClass<UMontageSystemComponent>())
            {
                MontageComp->PlayMontage(StationAnimation);
            }
        }
    }

    const FNamedCraftingRecipe& NamedRecipe = ActiveRecipeList[SelectedRecipeIndex];
    TMap<FGameplayTag,int32> MainToUse;
    if (NamedRecipe.RecipeData.RequiredMainMaterialCount > 0)
    {
        MainToUse = CurrentSelectedMainIngredients;
    }
    
    FCraftingResult CraftResult = Sys->Client_PreCraftCheck(Player, NamedRecipe.RecipeData, MainToUse);
    if (CraftResult.bWasSuccessful)
    {
        /*if (CraftingOutputBoxWidget)
        {
            CraftingOutputBoxWidget->TryAddItem(CraftResult.ItemTemplateClass, CraftResult.Rarity, CraftAmount);
        }*/
        Sys->RequestServerCraft(Player, CurrentStationActorRef, NamedRecipe.RecipeRowName, MainToUse);
    }
    
    ClearSelectedMainIngredients();
    RefreshAll();
}

void UCraftingWidget::HandleRecipeSelectedFromList(UCraftingRecipeListItemData* SelectedItemData)
{
    if (!SelectedItemData) return;

    for (int32 i = 0; i < ActiveRecipeList.Num(); ++i)
    {
        if (ActiveRecipeList[i].RecipeRowName == SelectedItemData->RecipeRowName)
        {
            SelectedRecipeIndex = i;
            SelectedRecipeName = SelectedItemData->RecipeRowName;
            ClearSelectedMainIngredients();
            RefreshAll();
            return;
        }
    }
}

void UCraftingWidget::HandleMainMaterialSelectionChanged(const FSelectedIngredientsMapWrapper& Wrapper)
{
    CurrentSelectedMainIngredients = Wrapper.IngredientsMap;
    RefreshAll();
}

void UCraftingWidget::HandleCraftRequest()
{
    UE_LOG(LogTemp, Warning, TEXT("[DEBUG 3] Main Widget received OnCraftRequested! Calling AttemptCraftCurrentRecipe..."));
    AttemptCraftCurrentRecipe();
}