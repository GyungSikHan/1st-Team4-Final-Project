// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSlotSingleWidget.h"

#include "EquipmentEntryWidget.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Managers/InventoryEquipmentManagerComponent.h"
#include "Managers/ItemManagerComponent.h"
#include "UI/Data/EmberUIData.h"
#include "UI/Inventory/ItemWidgets/ItemDragDrop.h"


UEquipmentSlotSingleWidget::UEquipmentSlotSingleWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UEquipmentSlotSingleWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (Image_BaseIcon)
	{
		Image_BaseIcon->SetBrushFromTexture(BaseIconTexture, true);
	}
}

bool UEquipmentSlotSingleWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);

	if (bAlreadyHovered)
		return true;
	
	bAlreadyHovered = true;

	UItemDragDrop* ItemDragDrop = Cast<UItemDragDrop>(InOperation);
	if (ItemDragDrop == nullptr)
		return false;

	UItemInstance* FromItemInstance = ItemDragDrop->FromItemInstance;
	if (FromItemInstance == nullptr)
		return false;

	bool bIsValid = false;
	if (UInventoryManagerComponent* FromInventoryManager = ItemDragDrop->FromInventoryManager)
	{
		if (InventoryEquipmentManager->IsSlotEquipped(EquipmentSlotType))
		{
			FIntPoint OutToItemSlotPos;
			bIsValid = InventoryEquipmentManager->CanSwapEquipment(FromInventoryManager, ItemDragDrop->FromItemSlotPos, EquipmentSlotType, OutToItemSlotPos);
		}
		else
		{
			bIsValid = InventoryEquipmentManager->CanMoveOrMergeEquipment(FromInventoryManager, ItemDragDrop->FromItemSlotPos, EquipmentSlotType) > 0;
		}
	}
	else if (UInventoryEquipmentManagerComponent* FromInventoryEquipmentManager = ItemDragDrop->FromInventoryEquipmentManager)
	{
		if (InventoryEquipmentManager->GetItemInstance(EquipmentSlotType))
		{
			bIsValid = InventoryEquipmentManager->CanSwapEquipment(FromInventoryEquipmentManager, ItemDragDrop->FromEquipmentSlotType, EquipmentSlotType);
		}
		else
		{
			bIsValid = InventoryEquipmentManager->CanMoveOrMergeEquipment(FromInventoryEquipmentManager, ItemDragDrop->FromEquipmentSlotType, EquipmentSlotType) > 0;
		}
	}
	
	// TODO: 장비창 내부에서의 아이템 이동 구현
	
	ChangeSlotState(bIsValid);

	return true;
}

bool UEquipmentSlotSingleWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	OnDragEnded();

	UItemDragDrop* ItemDragDrop = Cast<UItemDragDrop>(InOperation);
	if (ItemDragDrop == nullptr)
		return false; 

	if (UItemEntryWidget* FromEntryWidget = ItemDragDrop->FromEntryWidget)
	{
		FromEntryWidget->RefreshWidgetOpacity(true);
	}

	UItemInstance* FromItemInstance = ItemDragDrop->FromItemInstance;
	if (FromItemInstance == nullptr)
		return false;

	UItemManagerComponent* ItemManager = GetOwningPlayer()->FindComponentByClass<UItemManagerComponent>();
	if (ItemManager == nullptr)
		return false;
	
	if (EquipmentSlotType == EEquipmentSlotType::Count)
		return false;

	if (UInventoryManagerComponent* FromInventoryManager = ItemDragDrop->FromInventoryManager)
	{
		ItemManager->Server_InventoryToEquipment(FromInventoryManager, ItemDragDrop->FromItemSlotPos, InventoryEquipmentManager, EquipmentSlotType);
	}
	else if (UInventoryEquipmentManagerComponent* FromInventoryEquipmentManager = ItemDragDrop->FromInventoryEquipmentManager)
	{
		ItemManager->Server_EquipmentToEquipment(FromInventoryEquipmentManager, ItemDragDrop->FromEquipmentSlotType, InventoryEquipmentManager, EquipmentSlotType);
	}
	
	return true;
}

void UEquipmentSlotSingleWidget::OnDragEnded()
{
	Super::OnDragEnded();

	Image_Red->SetVisibility(ESlateVisibility::Hidden);
	Image_Green->SetVisibility(ESlateVisibility::Hidden);
}

void UEquipmentSlotSingleWidget::OnEquipmentEntryChanged(UItemInstance* InItemInstance, int32 InItemCount)
{
	if (EntryWidget)
	{
		Overlay_Entry->RemoveChild(EntryWidget);
		EntryWidget = nullptr;
	}

	if (InItemInstance)
	{
		TSubclassOf<UEquipmentEntryWidget> EntryWidgetClass = UEmberUIData::Get().EquipmentEntryWidgetClass;
		EntryWidget = CreateWidget<UEquipmentEntryWidget>(GetOwningPlayer(), EntryWidgetClass);
		
		UOverlaySlot* OverlaySlot = Overlay_Entry->AddChildToOverlay(EntryWidget);
		OverlaySlot->SetHorizontalAlignment(HAlign_Fill);
		OverlaySlot->SetVerticalAlignment(VAlign_Fill);
		
		EntryWidget->Init(InItemInstance, InItemCount, EquipmentSlotType, InventoryEquipmentManager);

		Image_BaseIcon->SetRenderOpacity(0.f);
	}
	else
	{
		Image_BaseIcon->SetRenderOpacity(1.f);
	}
}

void UEquipmentSlotSingleWidget::ChangeSlotState(bool bIsValid)
{
	if (bIsValid)
	{
		Image_Red->SetVisibility(ESlateVisibility::Hidden);
		Image_Green->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Image_Red->SetVisibility(ESlateVisibility::Visible);
		Image_Green->SetVisibility(ESlateVisibility::Hidden);
	}
}
