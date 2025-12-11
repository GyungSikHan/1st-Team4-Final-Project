// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"

#include "InventoryValidWidget.generated.h"

enum class ESlotState : uint8;
class USizeBox;
class UImage;
class UImage;

/**
 * 
 */
UCLASS()
class EMBER_API UInventoryValidWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

public:
	void ChangeSlotState(ESlotState SlotState);
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Red;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Green;
};
