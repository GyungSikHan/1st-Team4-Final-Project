// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Inventory/Widgets/HoverWidget.h"
#include "ItemInstance.h"

#include "ItemHoverWidget.generated.h"

class UItemHoverEntryWidget;

UCLASS()
class EMBER_API UItemHoverWidget : public UHoverWidget
{
	GENERATED_BODY()

public:
	UItemHoverWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	void RefreshUI(UItemInstance* HoveredItemInstance);

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UItemHoverEntryWidget> HoverWidget_Left;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UItemHoverEntryWidget> HoverWidget_Right;
};
