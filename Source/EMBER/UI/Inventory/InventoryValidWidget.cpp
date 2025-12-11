// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryValidWidget.h"

#include "GameFlag.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "UI/Data/EmberUIData.h"

void UInventoryValidWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FIntPoint UnitInventorySlotSize = UEmberUIData::Get().UnitInventorySlotSize;
	SizeBox_Root->SetWidthOverride(UnitInventorySlotSize.X);
	SizeBox_Root->SetHeightOverride(UnitInventorySlotSize.Y);
}

void UInventoryValidWidget::ChangeSlotState(ESlotState SlotState)
{
	switch (SlotState)
	{
	case ESlotState::Default:
		Image_Red->SetVisibility(ESlateVisibility::Hidden);
		Image_Green->SetVisibility(ESlateVisibility::Hidden);
		break;
	case ESlotState::Invalid:
		Image_Red->SetVisibility(ESlateVisibility::Visible);
		Image_Green->SetVisibility(ESlateVisibility::Hidden);
		break;
	case ESlotState::Valid:
		Image_Red->SetVisibility(ESlateVisibility::Hidden);
		Image_Green->SetVisibility(ESlateVisibility::Visible);
		break;
	}
}


