// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class USizeBox;

/**
 * 
 */
UCLASS()
class EMBER_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UInventorySlotWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	//~UUserWidget Overrides
	virtual void NativeOnInitialized() override;
	//~End of UUserWidget Overrides

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;
};
