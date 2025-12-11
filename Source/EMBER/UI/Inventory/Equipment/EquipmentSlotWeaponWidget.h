// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EquipmentSlotSingleWidget.h"
#include "EquipmentSlotWeaponWidget.generated.h"

class UCommonVisibilitySwitcher;

/**
 * 
 */
UCLASS()
class EMBER_API UEquipmentSlotWeaponWidget : public UEquipmentSlotWidget
{
	GENERATED_BODY()

public:
	UEquipmentSlotWeaponWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonVisibilitySwitcher> Switcher_WeaponHandEntry;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> Overlay_LeftHandEntry;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> Overlay_RightHandEntry;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> Overlay_TwoHandEntry;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Red_LeftHand;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Green_LeftHand;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Red_RightHand;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Green_RightHand;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Red_TwoHand;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Green_TwoHand;
};
