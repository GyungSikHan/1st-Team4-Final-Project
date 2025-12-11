// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFlag.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentSlotWidget.generated.h"

class UInventoryEquipmentManagerComponent;

/**
 * 
 */
UCLASS()
class EMBER_API UEquipmentSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UEquipmentSlotWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	void Init(EEquipmentSlotType InEquipmentSlotType, UInventoryEquipmentManagerComponent* InInventoryEquipmentManager);
	
protected:
	//~UUserWidget Overrides
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void OnDragEnded();
	//~End of UUserWidget Overrides
	
protected:
	UPROPERTY()
	TObjectPtr<UInventoryEquipmentManagerComponent> InventoryEquipmentManager;
	
	EEquipmentSlotType EquipmentSlotType = EEquipmentSlotType::Count;
	bool bAlreadyHovered = false;
};
