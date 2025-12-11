// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonActivatableWidget.h"
#include "EmberActivatableWidget.generated.h"

UENUM(BlueprintType)
enum class EEmberWidgetInputMode : uint8
{
	Default,
	GameAndUI,
	Game,
	UI
};
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class EMBER_API UEmberActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UEmberActivatableWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	//~UCommonActivatableWidget interface
	virtual void NativeOnInitialized() override;
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	//~End of UCommonActivatableWidget interface
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EEmberWidgetInputMode InputConfig = EEmberWidgetInputMode::Default;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	EMouseLockMode MouseLockMode = EMouseLockMode::LockAlways;
};
