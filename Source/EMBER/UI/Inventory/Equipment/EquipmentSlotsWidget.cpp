// Fill out your copyright notice in the Description page of Project Settings.

#include "EquipmentSlotsWidget.h"

#include "EquipmentSlotHandWidget.h"
#include "Managers/InventoryEquipmentManagerComponent.h"

class UGameplayMessageSubsystem;

UEquipmentSlotsWidget::UEquipmentSlotsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UEquipmentSlotsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	EquipmentSlotWidgets = {
		// Hand
		{EEquipmentSlotType::Primary_LeftHand,     Widget_Hand_Left},
		{EEquipmentSlotType::Primary_RightHand,    Widget_Hand_Right},

		// Armor
		{EEquipmentSlotType::Helmet,               Widget_Armor_Helmet},
		{EEquipmentSlotType::Shoulder,             Widget_Armor_Shoulder},
		{EEquipmentSlotType::Chest,                Widget_Armor_Chest},
		{EEquipmentSlotType::Cloak,                Widget_Armor_Cloak},
		{EEquipmentSlotType::Gloves,               Widget_Armor_Gloves},
		{EEquipmentSlotType::Pants,                Widget_Armor_Pants},
		{EEquipmentSlotType::Foot,                 Widget_Armor_Foot},

		// Utility
		{EEquipmentSlotType::Utility_Primary,      Widget_Utility_Primary},
		{EEquipmentSlotType::Utility_Secondary,    Widget_Utility_Secondary},
		{EEquipmentSlotType::Utility_Tertiary,     Widget_Utility_Tertiary},
		{EEquipmentSlotType::Utility_Quaternary,   Widget_Utility_Quaternary}
	};
}

void UEquipmentSlotsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageListenerHandle = MessageSubsystem.RegisterListener(MessageChannelTag, this, &ThisClass::ConstructUI);
}

void UEquipmentSlotsWidget::NativeDestruct()
{
	DestructUI();
	
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(MessageListenerHandle);

	Super::NativeDestruct();
}

void UEquipmentSlotsWidget::ConstructUI(FGameplayTag Channel, const FEquipmentInitializeMessage& Message)
{
	if (Message.InventoryEquipmentManager == nullptr)
		return;

	InventoryEquipmentManager = Message.InventoryEquipmentManager;

	for (const auto& Elem  : EquipmentSlotWidgets)
	{
		if (UEquipmentSlotSingleWidget* EquipmentSlotSingleWidget = Elem.Value)
		{
			EEquipmentSlotType EquipmentSlotType = Elem.Key;
			EquipmentSlotSingleWidget->Init(EquipmentSlotType, InventoryEquipmentManager);
		}
	}

	const TArray<FEquipmentEntry>& Entries = InventoryEquipmentManager->GetAllEntries();
	for (int32 i = 0; i < Entries.Num(); i++)
	{
		const FEquipmentEntry& Entry = Entries[i];
		if (UItemInstance* ItemInstance = Entry.GetItemInstance())
		{
			OnEquipmentEntryChanged((EEquipmentSlotType)i, ItemInstance, Entry.GetItemCount());
		}
	}

	EquipmentChangedDelegateHandle = InventoryEquipmentManager->OnEquipmentEntryChanged.AddUObject(this, &ThisClass::OnEquipmentEntryChanged);
}

void UEquipmentSlotsWidget::DestructUI()
{
	if (InventoryEquipmentManager)
	{
		InventoryEquipmentManager->OnEquipmentEntryChanged.Remove(EquipmentChangedDelegateHandle);
		EquipmentChangedDelegateHandle.Reset();
	}

	for (const auto& Elem  : EquipmentSlotWidgets)
	{
		if (UEquipmentSlotSingleWidget* EquipmentSlotSingleWidget = Elem.Value)
		{
			EquipmentSlotSingleWidget->OnEquipmentEntryChanged(nullptr, 0);
		}
	}
}

void UEquipmentSlotsWidget::OnEquipmentEntryChanged(EEquipmentSlotType EquipmentSlotType, UItemInstance* ItemInstance, int32 ItemCount)
{
	if (EquipmentSlotType == EEquipmentSlotType::Count)
		return;

	UEquipmentSlotSingleWidget* EquipmentSlotSingleWidget = EquipmentSlotWidgets.FindRef(EquipmentSlotType);
	if (EquipmentSlotSingleWidget == nullptr)
		return;

	EquipmentSlotSingleWidget->OnEquipmentEntryChanged(ItemInstance, ItemCount);
}
