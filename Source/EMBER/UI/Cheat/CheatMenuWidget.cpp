// Fill out your copyright notice in the Description page of Project Settings.


#include "CheatMenuWidget.h"

#include "CheatEntryWidget.h"
#include "CheatListWidget.h"
#include "ItemTemplate.h"
#include "Components/Button.h"
#include "Fragments/ItemFragment_Equipable.h"
#include "System/EmberAssetManager.h"

UCheatMenuWidget::UCheatMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCheatMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	CheatEntryWidgetClass = UEmberAssetManager::GetSubclassByName<UCheatEntryWidget>("CheatEntryWidgetClass");
	Button_Exit->OnClicked.AddUniqueDynamic(this, &ThisClass::OnExitButtonClicked);
}

void UCheatMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const UEmberItemData& ItemData = UEmberAssetManager::Get().GetItemData();

	TArray<TSubclassOf<UItemTemplate>> ItemTemplateClasses;
	ItemData.GetAllItemTemplateClasses(ItemTemplateClasses);
	
	for (TSubclassOf<UItemTemplate> ItemTemplateClass : ItemTemplateClasses)
	{
		UItemTemplate* ItemTemplate = ItemTemplateClass.GetDefaultObject();
		if (const UItemFragment_Equipable* EquippableFragment = ItemTemplate->FindFragmentByClass<UItemFragment_Equipable>())
		{
			if (EquippableFragment->EquipmentType == EEquipmentType::Weapon)
			{
				UCheatEntryWidget* CheatEntryWidget = CreateWidget<UCheatEntryWidget>(GetOwningPlayer(), CheatEntryWidgetClass);
				CheatEntryWidget->InitializeUI(ECheatEntryType::PrimaryWeapon, ItemTemplateClass, nullptr);
				CheatList_PrimaryWeapon->AddEntry(CheatEntryWidget);
			}
			else if (EquippableFragment->EquipmentType == EEquipmentType::Utility)
			{
				UCheatEntryWidget* CheatEntryWidget = CreateWidget<UCheatEntryWidget>(GetOwningPlayer(), CheatEntryWidgetClass);
				CheatEntryWidget->InitializeUI(ECheatEntryType::Utility, ItemTemplateClass, nullptr);
				CheatList_Utility->AddEntry(CheatEntryWidget);
			}
			
			else if (EquippableFragment->EquipmentType == EEquipmentType::Armor)
			{
				UCheatEntryWidget* CheatEntryWidget = CreateWidget<UCheatEntryWidget>(GetOwningPlayer(), CheatEntryWidgetClass);
				CheatEntryWidget->InitializeUI(ECheatEntryType::Armor, ItemTemplateClass, nullptr);
				CheatList_Armor->AddEntry(CheatEntryWidget);
			}
			else if (EquippableFragment->EquipmentType == EEquipmentType::Crafting)
			{
				UCheatEntryWidget* CheatEntryWidget = CreateWidget<UCheatEntryWidget>(GetOwningPlayer(), CheatEntryWidgetClass);
				CheatEntryWidget->InitializeUI(ECheatEntryType::Crafting, ItemTemplateClass, nullptr);
				CheatList_Crafting->AddEntry(CheatEntryWidget);
			}
			//UE_LOG(LogTemp, Warning, L"%d" EquippableFragment->EquipmentType);
		}
	}
}

void UCheatMenuWidget::OnExitButtonClicked()
{
	RemoveFromParent();
}
