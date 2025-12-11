// Fill out your copyright notice in the Description page of Project Settings.

#include "CheatListWidget.h"

#include "CheatEntryWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

UCheatListWidget::UCheatListWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCheatListWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	Text_Title->SetText(TitleText);
}

void UCheatListWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	ScrollBox_Entries->ClearChildren();
}

void UCheatListWidget::AddEntry(UCheatEntryWidget* EntryWidget)
{
	ScrollBox_Entries->AddChild(EntryWidget);
}
