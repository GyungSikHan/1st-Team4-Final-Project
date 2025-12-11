// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberActivatableWidget.h"

UEmberActivatableWidget::UEmberActivatableWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UEmberActivatableWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

TOptional<FUIInputConfig> UEmberActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
	case EEmberWidgetInputMode::GameAndUI:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode, MouseLockMode);
	case EEmberWidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode, MouseLockMode);
	case EEmberWidgetInputMode::UI:
		return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture, MouseLockMode);
	case EEmberWidgetInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
}
