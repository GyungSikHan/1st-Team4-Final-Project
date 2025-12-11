// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CheatListWidget.generated.h"

class UCheatEntryWidget;
class UTextBlock;
class UScrollBox;

/**
 * 
 */
UCLASS()
class EMBER_API UCheatListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UCheatListWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~UUserWidget Overrides
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	//~End of UUserWidget Overrides

public:
	void AddEntry(UCheatEntryWidget* EntryWidget);
	
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Title;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_Entries;

protected:
	UPROPERTY(EditAnywhere)
	FText TitleText;
};
