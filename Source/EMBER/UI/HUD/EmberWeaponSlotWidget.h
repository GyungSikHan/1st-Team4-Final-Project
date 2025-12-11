// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Managers/EquipmentManagerComponent.h"
#include "EmberWeaponSlotWidget.generated.h"

class UD1ItemTemplate;
class UImage;
class UOverlay;
class UTextBlock;
class UCommonVisibilitySwitcher;
class UInventoryEquipmentManagerComponent;

UCLASS()
class EMBER_API UEmberWeaponSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UEmberWeaponSlotWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void OnEquipmentEntryChanged(EEquipmentSlotType EquipmentSlotType, UItemInstance* ItemInstance, int32 ItemCount);
	void OnEquipStateChanged(EEquipState PrevEquipState, EEquipState NewEquipState);
	
public:
	UPROPERTY(EditAnywhere)
	EWeaponSlotType WidgetWeaponSlotType = EWeaponSlotType::Count;
	
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonVisibilitySwitcher> Switcher_Slots;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_SlotNumber;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_OneSlot;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_OneSlot_Count;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_TwoSlot_Left;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_TwoSlot_Right;

private:
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> Animation_ExpandSlot;
	
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> Animation_ShowCrossLine;

private:
	UPROPERTY()
	TObjectPtr<UInventoryEquipmentManagerComponent> InventoryEquipmentManager;

	UPROPERTY()
	TObjectPtr<UEquipmentManagerComponent> EquipmentManager;
	
private:
	FDelegateHandle EntryChangedDelegateHandle;
	FDelegateHandle EquipStateChangedDelegateHandle;
	
private:
	UPROPERTY()
	TWeakObjectPtr<UItemInstance> WeaponItemInstance;
};
