// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CheatMenuWidget.generated.h"

class UCheatEntryWidget;
class UCheatListWidget;
class UButton;

/**
 * 
 */
UCLASS()
class EMBER_API UCheatMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UCheatMenuWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~UUSerWidget Overrides
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	//~End of UUserWidget Overrides

private:
	UFUNCTION()
	void OnExitButtonClicked();

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCheatEntryWidget> CheatEntryWidgetClass;
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCheatListWidget> CheatList_PrimaryWeapon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCheatListWidget> CheatList_Utility;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCheatListWidget> CheatList_Armor;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCheatListWidget> CheatList_Crafting;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Exit;
};
