// Fill out your copyright notice in the Description page of Project Settings.


#include "CheatEntryWidget.h"

#include "GameFlag.h"
#include "ItemTemplate.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Managers/InventoryManagerComponent.h"
#include "Player/EmberPlayerController.h"
#include "Player/EmberPlayerState.h"

class AEmberPlayerController;

UCheatEntryWidget::UCheatEntryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCheatEntryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_Entry->OnClicked.AddUniqueDynamic(this, &ThisClass::OnButtonClicked);
}

void UCheatEntryWidget::InitializeUI(ECheatEntryType InCheatEntryType, TSubclassOf<UItemTemplate> InItemTemplateClass, TSoftObjectPtr<UAnimMontage> InAnimMontage)
{
	CheatEntryType = InCheatEntryType;
	
	switch (CheatEntryType)
	{
	case ECheatEntryType::PrimaryWeapon:
	case ECheatEntryType::SecondaryWeapon:
	case ECheatEntryType::Armor:
	case ECheatEntryType::Utility:
	case ECheatEntryType::Crafting:
		ItemTemplateClass = InItemTemplateClass;
		if (ItemTemplateClass)
		{
			if (UItemTemplate* ItemTemplate = Cast<UItemTemplate>(ItemTemplateClass->GetDefaultObject()))
			{
				Text_Entry->SetText(ItemTemplate->DisplayName);
				Image_Entry->SetBrushFromTexture(ItemTemplate->IconTexture, true);
			}
		}
		break;
	}
}

void UCheatEntryWidget::OnButtonClicked()
{
	AEmberPlayerController* EmberPlayerController = GetOwningPlayer<AEmberPlayerController>();
	if (EmberPlayerController == nullptr)
		return;

	switch (CheatEntryType)
	{
		case ECheatEntryType::PrimaryWeapon:
		case ECheatEntryType::Utility:
			EmberPlayerController->Server_EquipWeapon(EWeaponSlotType::Primary, ItemTemplateClass);
			break;
		case ECheatEntryType::Armor:
			EmberPlayerController->Server_EquipWeapon(EWeaponSlotType::Primary, ItemTemplateClass);
			break;
	}
}

void UCheatEntryWidget::OnButtonRightClicked()
{
	AEmberPlayerState* PlayerState = Cast<AEmberPlayerState>(GetOwningPlayerState());
	if (PlayerState == nullptr)
		return;
	
	EItemRarity RandomItemRarity = (EItemRarity)FMath::RandRange(0, (int32)EItemRarity::Count - 1);
	PlayerState->Server_AddInventoryItem(ItemTemplateClass, RandomItemRarity, 1);
}