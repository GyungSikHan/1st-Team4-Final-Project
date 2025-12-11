// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentEntryWidget.h"

#include "ItemInstance.h"
#include "ItemTemplate.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Managers/ItemManagerComponent.h"
#include "UI/Data/EmberItemData.h"
#include "UI/Data/EmberUIData.h"
#include "UI/Inventory/ItemWidgets/ItemDragDrop.h"
#include "UI/Inventory/ItemWidgets/ItemDragWidget.h"

class UItemDragDrop;

UEquipmentEntryWidget::UEquipmentEntryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UEquipmentEntryWidget::Init(UItemInstance* InItemInstance, int32 InItemCount, EEquipmentSlotType InEquipmentSlotType, UInventoryEquipmentManagerComponent* InInventoryEquipmentManager)
{
	if (InEquipmentSlotType == EEquipmentSlotType::Count)
		return;

	RefreshUI(InItemInstance, InItemCount);
	
	EquipmentSlotType = InEquipmentSlotType;
	InventoryEquipmentManager = InInventoryEquipmentManager;
}

FReply UEquipmentEntryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (Reply.IsEventHandled() == false && UWidgetBlueprintLibrary::IsDragDropping() && InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if (UItemManagerComponent* ItemManager = GetOwningPlayer()->FindComponentByClass<UItemManagerComponent>())
		{
			// ItemManager->Server_QUickFromEquipment();
			return FReply::Handled();
		}
	}
	
	return Reply;
}

void UEquipmentEntryWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	FIntPoint UnitInventorySlotSize = UEmberUIData::Get().UnitInventorySlotSize;
	const UItemTemplate& ItemTemplate = UEmberItemData::Get().FindItemTemplateByID(ItemInstance->GetItemTemplateID());

	TSubclassOf<UItemDragWidget> DragWidgetClass = UEmberUIData::Get().ItemDragWidgetClass;
	UItemDragWidget* DragWidget = CreateWidget<UItemDragWidget>(GetOwningPlayer(), DragWidgetClass);
	FVector2D DragWidgetSize = FVector2D(ItemTemplate.SlotCount.X * UnitInventorySlotSize.X, ItemTemplate.SlotCount.Y * UnitInventorySlotSize.Y);
	DragWidget->Init(DragWidgetSize, ItemTemplate.IconTexture, ItemCount);

	UItemDragDrop* ItemDragDrop = NewObject<UItemDragDrop>();
	ItemDragDrop->DefaultDragVisual = DragWidget;
	ItemDragDrop->Pivot = EDragPivot::CenterCenter;
	ItemDragDrop->FromEntryWidget = this;
	ItemDragDrop->FromInventoryEquipmentManager = InventoryEquipmentManager;
	ItemDragDrop->FromEquipmentSlotType = EquipmentSlotType;
	ItemDragDrop->FromItemInstance = ItemInstance;
	ItemDragDrop->DeltaWidgetPos = (DragWidgetSize / 2.f) - (UnitInventorySlotSize / 2.f);
	OutOperation = ItemDragDrop;
}
