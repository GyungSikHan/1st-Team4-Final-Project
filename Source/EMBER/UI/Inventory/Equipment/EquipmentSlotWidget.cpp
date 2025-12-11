// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSlotWidget.h"

UEquipmentSlotWidget::UEquipmentSlotWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UEquipmentSlotWidget::Init(EEquipmentSlotType InEquipmentSlotType, UInventoryEquipmentManagerComponent* InInventoryEquipmentManager)
{
	check(InEquipmentSlotType != EEquipmentSlotType::Count && InInventoryEquipmentManager != nullptr);

	EquipmentSlotType = InEquipmentSlotType;
	InventoryEquipmentManager = InInventoryEquipmentManager;
}

void UEquipmentSlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	OnDragEnded();
}

void UEquipmentSlotWidget::OnDragEnded()
{
	bAlreadyHovered = false;
}
