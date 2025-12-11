// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemEntryWidget.h"

#include "ItemHoverWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI/Data/EmberItemData.h"
#include "ItemTemplate.h"
#include "ItemInstance.h"
#include "UI/Data/EmberUIData.h"

class UItemTemplate;

UItemEntryWidget::UItemEntryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UItemEntryWidget::NativeOnInitialized()
{
	Text_Count->SetText(FText::GetEmpty());
}

void UItemEntryWidget::NativeConstruct()
{
	Image_Hover->SetVisibility(ESlateVisibility::Hidden);
}

void UItemEntryWidget::NativeDestruct()
{
	if (HoverWidget)
	{
		HoverWidget->RemoveFromParent();
		HoverWidget = nullptr;
	}

	Super::NativeDestruct();
}

void UItemEntryWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	
	Image_Hover->SetVisibility(ESlateVisibility::Visible);

	if (HoverWidget == nullptr)
	{
		TSubclassOf<UItemHoverWidget> ItemHoverWidgetClass = UEmberUIData::Get().ItemHoverWidgetClass;
		HoverWidget = CreateWidget<UItemHoverWidget>(GetOwningPlayer(), ItemHoverWidgetClass);
	}

	if (HoverWidget)
	{
		HoverWidget->RefreshUI(ItemInstance);
		HoverWidget->AddToViewport();
	}
}

FReply UItemEntryWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	if (HoverWidget)
	{
		HoverWidget->SetPosition(InMouseEvent.GetScreenSpacePosition());
		return FReply::Handled();
	}

	return Reply;
}

void UItemEntryWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	Image_Hover->SetVisibility(ESlateVisibility::Hidden);
	
	if (HoverWidget)
	{
		HoverWidget->RemoveFromParent();
		HoverWidget = nullptr;
	}
}

FReply UItemEntryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		Reply.DetectDrag(GetCachedWidget().ToSharedRef(), EKeys::LeftMouseButton);
	}
	
	return Reply;
}

void UItemEntryWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	RefreshWidgetOpacity(false);
}

void UItemEntryWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	RefreshWidgetOpacity(true);
}

void UItemEntryWidget::RefreshUI(UItemInstance* NewItemInstance, int32 NewItemCount)
{
	if (NewItemInstance == nullptr || NewItemCount < 1)
		return;

	ItemInstance = NewItemInstance;
	ItemCount = NewItemCount;

	const UItemTemplate& ItemTemplate = UEmberItemData::Get().FindItemTemplateByID(ItemInstance->GetItemTemplateID());
	
	Image_Icon->SetBrushFromTexture(ItemTemplate.IconTexture, true);
	Text_Count->SetText(ItemCount <= 1 ? FText::GetEmpty() : FText::AsNumber(ItemCount));
}

void UItemEntryWidget::RefreshItemCount(int32 NewItemCount)
{
	if (NewItemCount < 1)
		return;
	
	ItemCount = NewItemCount;
	Text_Count->SetText(ItemCount <= 1 ? FText::GetEmpty() : FText::AsNumber(ItemCount));
}

void UItemEntryWidget::RefreshWidgetOpacity(bool bClearlyVisible)
{
	SetRenderOpacity(bClearlyVisible ? 1.0f : 0.5f);
}


