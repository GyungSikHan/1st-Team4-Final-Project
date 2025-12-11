// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemHoverWidget.h"

#include "ItemHoverEntryWidget.h"

UItemHoverWidget::UItemHoverWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UItemHoverWidget::RefreshUI(UItemInstance* HoveredItemInstance)
{
	HoverWidget_Left->RefreshUI(HoveredItemInstance);
	HoverWidget_Right->RefreshUI(nullptr);
}
