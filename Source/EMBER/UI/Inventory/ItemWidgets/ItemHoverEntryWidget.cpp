// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemHoverEntryWidget.h"

#include "ItemInstance.h"
#include "ItemTemplate.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Fragments/ItemFragment_Equipable.h"
#include "Fragments/ItemFragment_Equipable_Armor.h"
#include "Fragments/ItemFragment_Equipable_Utility.h"
#include "Fragments/ItemFragment_Equipable_Weapon.h"
#include "UI/Data/EmberItemData.h"
#include "UI/Data/EmberUIData.h"

#define LOCTEXT_NAMESPACE "ItemHoverWidget"

UItemHoverEntryWidget::UItemHoverEntryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UItemHoverEntryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (bIsEquippedWidget)
	{
		Text_IsEquipped->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Text_IsEquipped->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UItemHoverEntryWidget::RefreshUI(const UItemInstance* ItemInstance)
{
	if (ItemInstance == nullptr)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	SetVisibility(ESlateVisibility::HitTestInvisible);
	PlayAnimationForward(Animation_FadeIn);

	const UItemTemplate& ItemTemplate = UEmberItemData::Get().FindItemTemplateByID(ItemInstance->GetItemTemplateID());
	const EItemRarity ItemRarity = ItemInstance->GetItemRarity();

	FColor RarityColor = UEmberUIData::Get().GetRarityColor(ItemRarity);
	Text_IsEquipped->SetColorAndOpacity(RarityColor);
	
	// Default Visibility
	Text_AttributeModifiers->SetVisibility(ESlateVisibility::Collapsed);
	Text_AdditionalAttributeModifiers->SetVisibility(ESlateVisibility::Collapsed);
	Text_Description->SetVisibility(ESlateVisibility::Collapsed);

	// Display Name
	Text_DisplayName->SetText(ItemTemplate.DisplayName);
	Text_DisplayName->SetColorAndOpacity(RarityColor);

	// Background Color
	UTexture2D* RarityTexture = UEmberUIData::Get().GetHoverRarityTexture(ItemRarity);
	Image_DisplayName_Background->SetBrushFromTexture(RarityTexture);

	FText ItemRarityText;
	switch (ItemRarity)
	{
	case EItemRarity::Common:		ItemRarityText = LOCTEXT("ItemRarity-Common",		"Common");		break;
	case EItemRarity::Uncommon:		ItemRarityText = LOCTEXT("ItemRarity-Uncommon",		"Uncommon");	break;
	case EItemRarity::Rare:			ItemRarityText = LOCTEXT("ItemRarity-Rare",			"Rare");		break;
	case EItemRarity::Unique:		ItemRarityText = LOCTEXT("ItemRarity-Unique",		"Unique");		break;
	case EItemRarity::Legendary:	ItemRarityText = LOCTEXT("ItemRarity-Legendary",	"Legendary");	break;
	}
	Text_ItemRarity->SetText(ItemRarityText);
	Text_ItemRarity->SetColorAndOpacity(RarityColor);

	// Item Specifics
	FString DescriptionString = ItemTemplate.Description.ToString();

	if (const UItemFragment_Equipable* EquipableFragment = ItemTemplate.FindFragmentByClass<UItemFragment_Equipable>())
	{
		if (EquipableFragment->EquipmentType == EEquipmentType::Armor)
		{
			const UItemFragment_Equipable_Armor* ArmorFragment = Cast<UItemFragment_Equipable_Armor>(EquipableFragment);

			// Item Type
			FText ArmorTypeText;
			switch (ArmorFragment->ArmorType)
			{
			case EArmorType::Helmet:	ArmorTypeText = LOCTEXT("ArmorType-Helmet",	"Helmet");	break;
			case EArmorType::Shoulder:	ArmorTypeText = LOCTEXT("ArmorType-Shoulder", "Shoulder");	break;
			case EArmorType::Cloak:		ArmorTypeText = LOCTEXT("ArmorType-Cloak",	"Cloak");	break;
			case EArmorType::Chest:		ArmorTypeText = LOCTEXT("ArmorType-Chest",	"Chest");	break;
			case EArmorType::Gloves:	ArmorTypeText = LOCTEXT("ArmorType-Gloves",	"Gloves");	break;
			case EArmorType::Pants:		ArmorTypeText = LOCTEXT("ArmorType-Pants",	"Pants");	break;
			case EArmorType::Foot:		ArmorTypeText = LOCTEXT("ArmorType-Foot",	"Foot");	break;
			}
			Text_ItemType->SetText(ArmorTypeText);
		}
		else if (EquipableFragment->EquipmentType == EEquipmentType::Weapon)
		{
			const UItemFragment_Equipable_Weapon* WeaponFragment = Cast<UItemFragment_Equipable_Weapon>(EquipableFragment);

			// Item Type
			FText WeaponTypeText;
			switch (WeaponFragment->WeaponType)
			{
			case EWeaponType::OneHandSword:		WeaponTypeText = LOCTEXT("WeaponType-OneHandSword",	"One-Hand Sword");	break;
			case EWeaponType::TwoHandSword:		WeaponTypeText = LOCTEXT("WeaponType-TwoHandSword", "Two-Hand Sword");	break;
			case EWeaponType::Shield:		    WeaponTypeText = LOCTEXT("WeaponType-Shield",		"Shield");			break;
			}
			Text_ItemType->SetText(WeaponTypeText);
		}
		else if (EquipableFragment->EquipmentType == EEquipmentType::Utility)
		{
			const UItemFragment_Equipable_Utility* UtilityFragment = Cast<UItemFragment_Equipable_Utility>(EquipableFragment);
			
			// Item Type
			FText UtilityTypeText;
			switch (UtilityFragment->UtilityType)
			{
			case EUtilityType::Consumable:		UtilityTypeText = LOCTEXT("UtilityType-Drink",			"Drink");			break;
			case EUtilityType::LightSource: UtilityTypeText = LOCTEXT("UtilityType-LightSource",	"Light Source");	break;
			}
			Text_ItemType->SetText(UtilityTypeText);
		}
	}

	// Attribute Modifiers
	FString AttributeString;

	const FGameplayTagStackContainer& StackContainer = ItemInstance->GetAttributeContainer();
	for (const FEmberGameplayTagStack& Stack : StackContainer.GetStacks())
	{
		FString Left, Right;
		Stack.GetStackTag().ToString().Split(TEXT("."), &Left, &Right, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		AttributeString.Append(FString::Printf(TEXT("%s %d\n"), *Right, Stack.GetStackCount()));
	}

	if (AttributeString.IsEmpty() == false)
	{
		AttributeString.RemoveFromEnd(TEXT("\n"));
		Text_AttributeModifiers->SetText(FText::FromString(AttributeString));
		Text_AttributeModifiers->SetVisibility(ESlateVisibility::Visible);
	}

	// Item Description
	if (DescriptionString.IsEmpty() == false)
	{
		Text_Description->SetText(FText::FromString(DescriptionString));
		Text_Description->SetVisibility(ESlateVisibility::Visible);
	}
}

#undef LOCTEXT_NAMESPACE