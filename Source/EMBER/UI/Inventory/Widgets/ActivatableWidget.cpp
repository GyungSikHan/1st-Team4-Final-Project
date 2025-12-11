// Fill out your copyright notice in the Description page of Project Settings.

#include "ActivatableWidget.h"

#include "Input/EmberEnhancedPlayerInput.h"

UActivatableWidget::UActivatableWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

FReply UActivatableWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		if (UEmberEnhancedPlayerInput* PlayerInput = Cast<UEmberEnhancedPlayerInput>(PlayerController->PlayerInput))
		{
			FKey DeactivateKey = PlayerInput->GetKeyForAction(DeactivateInputAction);

			if (DeactivateKey.IsValid() && InKeyEvent.GetKey() == DeactivateKey && InKeyEvent.IsRepeat() == false)
			{
				DeactivateWidget();
				return FReply::Handled();
			}
		}
	}
	
	return Reply;
}
