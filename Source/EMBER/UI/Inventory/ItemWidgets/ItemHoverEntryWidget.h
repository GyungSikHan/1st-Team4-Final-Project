// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ItemHoverEntryWidget.generated.h"

class UImage;
class UTextBlock;
class UHorizontalBox;
class URichTextBlock;
class UItemInstance;

UCLASS()
class EMBER_API UItemHoverEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UItemHoverEntryWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void NativePreConstruct() override;
	
public:
	void RefreshUI(const UItemInstance* ItemInstance);

protected:
	UPROPERTY(EditAnywhere)
	bool bIsEquippedWidget = false;

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_DisplayName;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_IsEquipped;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_ItemRarity;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_ItemType;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_AttributeModifiers;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_AdditionalAttributeModifiers;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<URichTextBlock> Text_Description;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_DisplayName_Background;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> Animation_FadeIn;
};
