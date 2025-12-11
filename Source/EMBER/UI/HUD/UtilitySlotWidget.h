// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFlag.h"
#include "Blueprint/UserWidget.h"
#include "UtilitySlotWidget.generated.h"

enum class EUtilitySlotType : uint8;
class UItemInstance;
class UTextBlock;
class UImage;
class UInventoryEquipmentManagerComponent;
class UEquipmentManagerComponent;

UCLASS()
class EMBER_API UUtilitySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UUtilitySlotWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~ UUSerWidget Overrides
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//~ End of UUSerWidget Overrides

private:
	void OnEquipmentEntryChanged(EEquipmentSlotType EquipmentSlotType, UItemInstance* ItemInstance, int32 ItemCount);
	void OnEquipStateChanged(EEquipState PrevEquipState, EEquipState NewEquipState);

	
public:
	UPROPERTY(EditAnywhere)
	EUtilitySlotType WidgetUtilitySlotType = EUtilitySlotType::Count;
	
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_SlotNumber;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Highlight;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Count;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> Animation_Highlight_In;

private:
	UPROPERTY()
	TObjectPtr<UInventoryEquipmentManagerComponent> InventoryEquipmentManager;
	
	UPROPERTY()
	TObjectPtr<UEquipmentManagerComponent> EquipmentManager;

private:
	FDelegateHandle EntryChangedDelegateHandle;
	FDelegateHandle EquipStateChangedDelegateHandle;
};
