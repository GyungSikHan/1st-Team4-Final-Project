// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"

#include "ItemEntryWidget.generated.h"

class UItemInstance;
class UImage;
class UTextBlock;
class UItemHoverWidget;

UCLASS()
class EMBER_API UItemEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UItemEntryWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~UUserWidget Overrides
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	//~End of UUserWidget Overrides

public:
	void RefreshUI(UItemInstance* NewItemInstance, int32 NewItemCount);
	void RefreshItemCount(int32 NewItemCount);
	void RefreshWidgetOpacity(bool bClearlyVisible);

public:
	UItemInstance* GetItemInstance() const { return ItemInstance; }

protected:
	UPROPERTY()
	TObjectPtr<UItemInstance> ItemInstance;
	
	UPROPERTY()
	int32 ItemCount = 0;

	UPROPERTY()
	TObjectPtr<UItemHoverWidget> HoverWidget;
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_RarityCover;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Icon_Disable;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Hover;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Count;
};
