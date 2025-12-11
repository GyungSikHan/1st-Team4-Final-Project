// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFlag.h"
#include "EquipmentSlotWidget.h"

#include "EquipmentSlotSingleWidget.generated.h"

class UImage;
class UOverlay;
class UItemInstance;
class UEquipmentEntryWidget;

/**
 * 
 */
UCLASS()
class EMBER_API UEquipmentSlotSingleWidget : public UEquipmentSlotWidget
{
	GENERATED_BODY()

public:
	UEquipmentSlotSingleWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~UUserWidget Overrides
	virtual void NativePreConstruct() override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void OnDragEnded() override;
	//~End of UUSerWidget Overrides

public:
	void OnEquipmentEntryChanged(UItemInstance* InItemInstance, int32 InItemCount);
	
private:
	void ChangeSlotState(bool bIsValid);
	
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> BaseIconTexture;
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> Overlay_Entry;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_BaseIcon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Red;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Green;

private:
	UPROPERTY()
	TObjectPtr<UEquipmentEntryWidget> EntryWidget;
};
