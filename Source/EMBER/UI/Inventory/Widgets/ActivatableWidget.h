// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Common/EmberActivatableWidget.h"
#include "ActivatableWidget.generated.h"

class UInputAction;

UCLASS()
class EMBER_API UActivatableWidget : public UEmberActivatableWidget
{
	GENERATED_BODY()

public:
	UActivatableWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> DeactivateInputAction;
};
