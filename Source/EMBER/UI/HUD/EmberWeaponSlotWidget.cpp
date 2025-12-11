#include "EmberWeaponSlotWidget.h"

#include "CommonVisibilitySwitcher.h"
#include "EmberPlayerCharacter.h"
#include "ItemInstance.h"
#include "ItemTemplate.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Managers/InventoryEquipmentManagerComponent.h"
#include "UI/Data/EmberItemData.h"

UEmberWeaponSlotWidget::UEmberWeaponSlotWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UEmberWeaponSlotWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	FText SlotNumber = FText::AsNumber( (int32)WidgetWeaponSlotType + 1);
	if (Text_SlotNumber)
	{
		Text_SlotNumber->SetText(SlotNumber);
	}
}

void UEmberWeaponSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AEmberPlayerCharacter* EmberCharacter = Cast<AEmberPlayerCharacter>(GetOwningPlayerPawn());
	if (EmberCharacter == nullptr)
		return;

	EquipmentManager = EmberCharacter->GetComponentByClass<UEquipmentManagerComponent>();
	InventoryEquipmentManager = EmberCharacter->GetComponentByClass<UInventoryEquipmentManagerComponent>();
	if (InventoryEquipmentManager == nullptr || EquipmentManager == nullptr)
		return;
	
	EntryChangedDelegateHandle = InventoryEquipmentManager->OnEquipmentEntryChanged.AddUObject(this, &ThisClass::OnEquipmentEntryChanged);

	EEquipState CurrentEquipState = EquipmentManager->GetCurrentEquipState();
	OnEquipStateChanged(CurrentEquipState, CurrentEquipState);
	EquipStateChangedDelegateHandle = EquipmentManager->OnEquipStateChanged.AddUObject(this, &ThisClass::OnEquipStateChanged);
}

void UEmberWeaponSlotWidget::NativeDestruct()
{
	if (InventoryEquipmentManager)
	{
		InventoryEquipmentManager->OnEquipmentEntryChanged.Remove(EntryChangedDelegateHandle);
		EntryChangedDelegateHandle.Reset();
	}

	Super::NativeDestruct();
}

void UEmberWeaponSlotWidget::OnEquipmentEntryChanged(EEquipmentSlotType EquipmentSlotType, UItemInstance* ItemInstance, int32 ItemCount)
{
	if (EquipmentSlotType == EEquipmentSlotType::Primary_RightHand)
	{
		if (ItemInstance)
		{
			const UItemTemplate& ItemTemplate = UEmberItemData::Get().FindItemTemplateByID(ItemInstance->GetItemTemplateID());
			Image_TwoSlot_Left->SetBrushFromTexture(ItemTemplate.IconTexture, true);
			Image_TwoSlot_Left->SetVisibility(ESlateVisibility::HitTestInvisible);

			if (Switcher_Slots->GetActiveWidgetIndex() != 1)
			{
				PlayAnimationForward(Animation_ShowCrossLine);
				Switcher_Slots->SetActiveWidgetIndex(1);
			}
		}
		else
		{
			Image_TwoSlot_Left->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else if (EquipmentSlotType == EEquipmentSlotType::Primary_LeftHand)
	{
		if (ItemInstance)
		{
			const UItemTemplate& ItemTemplate = UEmberItemData::Get().FindItemTemplateByID(ItemInstance->GetItemTemplateID());
			Image_TwoSlot_Right->SetBrushFromTexture(ItemTemplate.IconTexture, true);
			Image_TwoSlot_Right->SetVisibility(ESlateVisibility::HitTestInvisible);

			if (Switcher_Slots->GetActiveWidgetIndex() != 1)
			{
				PlayAnimationForward(Animation_ShowCrossLine);
				Switcher_Slots->SetActiveWidgetIndex(1);
			}
		}
		else
		{
			Image_TwoSlot_Right->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else if (EquipmentSlotType == EEquipmentSlotType::Primary_TwoHand)
	{
		WeaponItemInstance = nullptr;
		Text_OneSlot_Count->SetVisibility(ESlateVisibility::Hidden);

		if (ItemInstance)
		{
			const UItemTemplate& ItemTemplate = UEmberItemData::Get().FindItemTemplateByID(ItemInstance->GetItemTemplateID());
			Image_OneSlot->SetBrushFromTexture(ItemTemplate.IconTexture, true);
			Image_OneSlot->SetVisibility(ESlateVisibility::HitTestInvisible);

			if (Switcher_Slots->GetActiveWidgetIndex() != 0)
			{
				PlayAnimationReverse(Animation_ShowCrossLine);
				Switcher_Slots->SetActiveWidgetIndex(0);
			}
		}
	}
	else
	{
		Image_OneSlot->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UEmberWeaponSlotWidget::OnEquipStateChanged(EEquipState PrevEquipState, EEquipState NewEquipState)
{
	if (NewEquipState == EEquipState::Weapon_Primary)
	{
		PlayAnimationForward(Animation_ExpandSlot);
	}
	else if (PrevEquipState == EEquipState::Weapon_Primary)
	{
		PlayAnimationReverse(Animation_ExpandSlot);
	}
}
