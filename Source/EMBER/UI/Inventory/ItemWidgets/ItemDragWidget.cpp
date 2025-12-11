// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDragWidget.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

UItemDragWidget::UItemDragWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UItemDragWidget::Init(const FVector2D& InWidgetSize, UTexture2D* InItemIcon, int32 InItemCount)
{
	SizeBox_Root->SetWidthOverride(InWidgetSize.X);
	SizeBox_Root->SetHeightOverride(InWidgetSize.Y);

	Image_Icon->SetBrushFromTexture(InItemIcon, true);
	Text_Count->SetText((InItemCount >= 2) ? FText::AsNumber(InItemCount) : FText::GetEmpty());
}
