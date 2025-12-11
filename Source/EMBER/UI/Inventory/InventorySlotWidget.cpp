// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventorySlotWidget.h"

#include "Components/SizeBox.h"
#include "UI/Data/EmberUIData.h"

UInventorySlotWidget::UInventorySlotWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UInventorySlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FIntPoint UnitInventorySlotSize = UEmberUIData::Get().UnitInventorySlotSize;
	SizeBox_Root->SetWidthOverride(UnitInventorySlotSize.X);
	SizeBox_Root->SetHeightOverride(UnitInventorySlotSize.Y);
}
