// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventorySlotsWidget.h"

#include "UI/Data/EmberUIData.h"
#include "InventorySlotWidget.h"
#include "InventoryValidWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/Overlay.h"
#include "Managers/InventoryManagerComponent.h"
#include "InventoryEntryWidget.h"
#include "ItemInstance.h"
#include "ItemTemplate.h"
#include "ItemWidgets/ItemDragDrop.h"
#include "Managers/EquipmentManagerComponent.h"
#include "Managers/ItemManagerComponent.h"
#include "UI/Data/EmberItemData.h"

UInventorySlotsWidget::UInventorySlotsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TitleText = FText::FromString(TEXT("Title"));
}

void UInventorySlotsWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	Text_Title->SetText(TitleText);
}

void UInventorySlotsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageListenerHandle = MessageSubsystem.RegisterListener(MessageChannelTag, this, &ThisClass::ConstructUI);
}

void UInventorySlotsWidget::NativeDestruct()
{
	DestructUI();
	
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	MessageSubsystem.UnregisterListener(MessageListenerHandle);
	
	Super::NativeDestruct();
}

bool UInventorySlotsWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);

	UItemDragDrop* DragDrop = Cast<UItemDragDrop>(InOperation);
	if (DragDrop == nullptr)
		return false;

	FIntPoint UnitInventorySlotSize = UEmberUIData::Get().UnitInventorySlotSize;

	FVector2D MouseWidgetPos = GetSlotContainerGeometry().AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
	FVector2D ToWidgetPos = MouseWidgetPos - DragDrop->DeltaWidgetPos;
	FIntPoint ToItemSlotPos = FIntPoint(ToWidgetPos.X / UnitInventorySlotSize.X, ToWidgetPos.Y / UnitInventorySlotSize.Y);

	if (PrevDragOverSlotPos == ToItemSlotPos)
		return true;

	PrevDragOverSlotPos = ToItemSlotPos;

	UItemEntryWidget* FromEntryWidget = DragDrop->FromEntryWidget;
	UItemInstance* FromItemInstance = FromEntryWidget->GetItemInstance();
	if (FromItemInstance == nullptr)
		return false;

	const UItemTemplate& FromItemTemplate = UEmberItemData::Get().FindItemTemplateByID(FromItemInstance->GetItemTemplateID());
	const FIntPoint& FromItemSlotCount = FromItemTemplate.SlotCount;

	int32 MovableCount = 0;
	if (UInventoryManagerComponent* FromInventoryManager = DragDrop->FromInventoryManager)
	{
		MovableCount = InventoryManager->CanMoveOrMergeItem(FromInventoryManager, DragDrop->FromItemSlotPos, ToItemSlotPos);
	}
	else if (UInventoryEquipmentManagerComponent* FromEquipmentManager = DragDrop->FromInventoryEquipmentManager)
	{
		MovableCount = InventoryManager->CanMoveOrMergeItem(FromEquipmentManager, DragDrop->FromEquipmentSlotType, ToItemSlotPos);
	}

	ResetValidSlots();
	
	const FIntPoint& InventorySlotCount = InventoryManager->GetInventorySlotCount();
	
	const FIntPoint StartSlotPos = FIntPoint(FMath::Max(0, ToItemSlotPos.X), FMath::Max(0, ToItemSlotPos.Y));
	const FIntPoint EndSlotPos   = FIntPoint(FMath::Min(ToItemSlotPos.X + FromItemSlotCount.X, InventorySlotCount.X),
											 FMath::Min(ToItemSlotPos.Y + FromItemSlotCount.Y, InventorySlotCount.Y));

	ESlotState SlotState = (MovableCount > 0) ? ESlotState::Valid : ESlotState::Invalid;
	for (int32 y = StartSlotPos.Y; y < EndSlotPos.Y; y++)
	{
		for (int32 x = StartSlotPos.X; x < EndSlotPos.X; x++)
		{
			int32 Index = y * InventorySlotCount.X + x;
			if (UInventoryValidWidget* ValidWidget = ValidWidgets[Index])
			{
				ValidWidget->ChangeSlotState(SlotState);
			}
		}
	}
	
	return true;
}

bool UInventorySlotsWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	FinishDrag();
	
	FIntPoint UnitInventorySlotSize = UEmberUIData::Get().UnitInventorySlotSize;
	UItemDragDrop* DragDrop = Cast<UItemDragDrop>(InOperation);
	if (DragDrop == nullptr)
		return false;

	UItemEntryWidget* FromEntryWidget = DragDrop->FromEntryWidget;
	FromEntryWidget->RefreshWidgetOpacity(true);

	/* 인벤토리 드랍 좌표(x,y) 계산 */
	FVector2D MouseWidgetPos = GetSlotContainerGeometry().AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
	FVector2D ToWidgetPos = MouseWidgetPos - DragDrop->DeltaWidgetPos;
	FIntPoint ToItemSlotPos = FIntPoint(ToWidgetPos.X / UnitInventorySlotSize.X, ToWidgetPos.Y / UnitInventorySlotSize.Y);

	DragDrop->ToInventoryManager = InventoryManager;
	DragDrop->ToItemSlotPos = ToItemSlotPos;

	UItemManagerComponent* ItemManager = GetOwningPlayer()->FindComponentByClass<UItemManagerComponent>();
	check(ItemManager);

	if (UInventoryManagerComponent* FromInventoryManager =  DragDrop->FromInventoryManager)
	{
		ItemManager->Server_InventoryToInventory(FromInventoryManager, DragDrop->FromItemSlotPos, InventoryManager, ToItemSlotPos);
	}
	else if (UInventoryEquipmentManagerComponent* FromEquipmentManager = DragDrop->FromInventoryEquipmentManager)
	{
		ItemManager->Server_EquipmentToInventory(FromEquipmentManager, DragDrop->FromEquipmentSlotType, InventoryManager, ToItemSlotPos);
	}
	
	return true;
}

void UInventorySlotsWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	FinishDrag();
}

void UInventorySlotsWidget::ConstructUI(FGameplayTag Channel, const FInventoryInitializeMessage& Message)
{
	if (Message.InventoryManager == nullptr)
		return;

	InventoryManager = Message.InventoryManager;
		
	const FIntPoint& InventorySlotCount = InventoryManager->GetInventorySlotCount();
	const int32 InventorySlotNum = InventorySlotCount.X * InventorySlotCount.Y;
	SlotWidgets.SetNum(InventorySlotNum);
	EntryWidgets.SetNum(InventorySlotNum);
	ValidWidgets.SetNum(InventorySlotNum);
	
	for (int32 y = 0; y < InventorySlotCount.Y; y++)
	{
		for (int32 x = 0; x < InventorySlotCount.X; x++)
		{
			TSubclassOf<UInventorySlotWidget> SlotWidgetClass = UEmberUIData::Get().InventorySlotWidgetClass;
			UInventorySlotWidget* SlotWidget = CreateWidget<UInventorySlotWidget>(GetOwningPlayer(), SlotWidgetClass);
			SlotWidgets[y * InventorySlotCount.X + x] = SlotWidget;
			GridPanel_Slots->AddChildToUniformGrid(SlotWidget, y, x);

			TSubclassOf<UInventoryValidWidget> ValidWidgetClass = UEmberUIData::Get().InventoryValidWidgetClass;
			UInventoryValidWidget* ValidWidget = CreateWidget<UInventoryValidWidget>(GetOwningPlayer(), ValidWidgetClass);
			ValidWidgets[y * InventorySlotCount.X + x] = ValidWidget;
			GridPanel_ValidSlots->AddChildToUniformGrid(ValidWidget, y, x);
		}
	}

	const TArray<FInventoryEntry>& Entries = InventoryManager->GetAllEntries();
	for (int32 i = 0; i < Entries.Num(); i++)
	{
		const FInventoryEntry& Entry = Entries[i];
		if (UItemInstance* ItemInstance = Entry.GetItemInstance())
		{
			FIntPoint ItemSlotPos = FIntPoint(i % InventorySlotCount.X, i / InventorySlotCount.X);
			OnInventoryEntryChanged(ItemSlotPos, ItemInstance, Entry.GetItemCount());
		}
	}
	
	EntryChangedDelegateHandle = InventoryManager->OnInventoryEntryChanged.AddUObject(this, &ThisClass::OnInventoryEntryChanged);
}

void UInventorySlotsWidget::DestructUI()
{
	InventoryManager->OnInventoryEntryChanged.Remove(EntryChangedDelegateHandle);
	EntryChangedDelegateHandle.Reset();
	
	CanvasPanel_Entries->ClearChildren();
	EntryWidgets.Reset();
	
	GridPanel_Slots->ClearChildren();
	SlotWidgets.Reset();
}

void UInventorySlotsWidget::ResetValidSlots()
{
	for (UInventoryValidWidget* ValidWidget : ValidWidgets)
	{
		if (ValidWidget)
		{
			ValidWidget->ChangeSlotState(ESlotState::Default);
		}
	}
}

void UInventorySlotsWidget::FinishDrag()
{
	ResetValidSlots();
	PrevDragOverSlotPos = FIntPoint(-1, -1);
}

void UInventorySlotsWidget::OnInventoryEntryChanged(const FIntPoint& InItemSlotPos, UItemInstance* InItemInstance, int32 InItemCount)
{
	const FIntPoint& InventorySlotCount = InventoryManager->GetInventorySlotCount();
	int32 SlotIndex = InItemSlotPos.Y * InventorySlotCount.X + InItemSlotPos.X;

	if (UInventoryEntryWidget* EntryWidget = EntryWidgets[SlotIndex])
	{
		UItemInstance* ItemInstance = EntryWidget->GetItemInstance();
		if (ItemInstance && ItemInstance == InItemInstance)
		{
			EntryWidget->RefreshItemCount(InItemCount);
			return;
		}
		
		CanvasPanel_Entries->RemoveChild(EntryWidget);
		EntryWidgets[SlotIndex] = nullptr;
	}
	
	if (InItemInstance)
	{
		FIntPoint UnitInventorySlotSize = UEmberUIData::Get().UnitInventorySlotSize;
		
		TSubclassOf<UInventoryEntryWidget> EntryWidgetClass = UEmberUIData::Get().InventoryEntryWidgetClass;
		UInventoryEntryWidget* EntryWidget = CreateWidget<UInventoryEntryWidget>(GetOwningPlayer(), EntryWidgetClass);
		EntryWidgets[SlotIndex] = EntryWidget;
		
		EntryWidget->Init(this, InItemInstance, InItemCount);
		
		UCanvasPanelSlot* CanvasPanelSlot = CanvasPanel_Entries->AddChildToCanvas(EntryWidget);
		CanvasPanelSlot->SetAutoSize(true);
		CanvasPanelSlot->SetPosition(FVector2D(InItemSlotPos.X * UnitInventorySlotSize.X, InItemSlotPos.Y * UnitInventorySlotSize.Y));
	}
}

const FGeometry& UInventorySlotsWidget::GetSlotContainerGeometry() const
{
	return Overlay_Slots->GetCachedGeometry();
}
