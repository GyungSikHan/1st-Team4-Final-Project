// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemDragWidget.generated.h"

class UTexture2D;
class USizeBox;
class UImage;
class UTextBlock;

UCLASS()
class EMBER_API UItemDragWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UItemDragWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	void Init(const FVector2D& InWidgetSize, UTexture2D* InItemIcon, int32 InItemCount);
	
protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text_Count;
};
