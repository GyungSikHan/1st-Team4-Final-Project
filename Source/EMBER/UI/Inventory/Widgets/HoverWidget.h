// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "HoverWidget.generated.h"

class UCanvasPanel;
class UHorizontalBox;

UCLASS()
class EMBER_API UHoverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UHoverWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	void SetPosition(const FVector2D& AbsolutePosition);

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel_Root;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UHorizontalBox> HorizontalBox_Hovers;
};