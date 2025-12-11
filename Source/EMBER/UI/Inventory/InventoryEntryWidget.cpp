// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryEntryWidget.h"

#include "InventorySlotsWidget.h"
#include "UI/Data/EmberItemData.h"
#include "UI/Data/EmberUIData.h"
#include "ItemInstance.h"
#include "ItemTemplate.h"
#include "ItemWidgets/ItemDragWidget.h"
#include "ItemWidgets/ItemDragDrop.h"
#include "Components/SizeBox.h"

UInventoryEntryWidget::UInventoryEntryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UInventoryEntryWidget::Init(UInventorySlotsWidget* InSlotsWidget, UItemInstance* InItemInstance, int32 InItemCount)
{
	if (InSlotsWidget == nullptr || InItemInstance == nullptr)
		return;
	
	SlotsWidget = InSlotsWidget;

	// 슬롯 사이즈(X,Y) 설정하기
	FIntPoint UnitInventorySlotSize = UEmberUIData::Get().UnitInventorySlotSize;
	const UItemTemplate& ItemTemplate = UEmberItemData::Get().FindItemTemplateByID(InItemInstance->GetItemTemplateID());

	FVector2D WidgetSize = FVector2D(ItemTemplate.SlotCount.X * UnitInventorySlotSize.X, ItemTemplate.SlotCount.Y * UnitInventorySlotSize.Y);
	SizeBox_Root->SetWidthOverride(WidgetSize.X);
	SizeBox_Root->SetHeightOverride(WidgetSize.Y);

	// UI 업데이트하기
	RefreshUI(InItemInstance, InItemCount);
}

FReply UInventoryEntryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	FIntPoint UnitInventorySlotSize = UEmberUIData::Get().UnitInventorySlotSize;

	FVector2D MouseWidgetPos = SlotsWidget->GetSlotContainerGeometry().AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	FVector2D ItemWidgetPos = SlotsWidget->GetSlotContainerGeometry().AbsoluteToLocal(InGeometry.LocalToAbsolute(UnitInventorySlotSize / 2.f));
	FIntPoint ItemSlotPos = FIntPoint(ItemWidgetPos.X / UnitInventorySlotSize.X, ItemWidgetPos.Y / UnitInventorySlotSize.Y);

	CachedFromSlotPos = ItemSlotPos;
	CachedDeltaWidgetPos = MouseWidgetPos - ItemWidgetPos;
	
	return Reply;
}

void UInventoryEntryWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
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
	ItemDragDrop->Pivot = EDragPivot::TopLeft;
	ItemDragDrop->Offset = -((CachedDeltaWidgetPos + UnitInventorySlotSize / 2.f) / DragWidgetSize);
	ItemDragDrop->FromEntryWidget = this;
	ItemDragDrop->FromInventoryManager = SlotsWidget->GetInventoryManager();
	ItemDragDrop->FromItemSlotPos = CachedFromSlotPos;
	ItemDragDrop->FromItemInstance = ItemInstance;
	ItemDragDrop->DeltaWidgetPos = CachedDeltaWidgetPos;
	
	OutOperation = ItemDragDrop;
}
