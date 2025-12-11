// Fill out your copyright notice in the Description page of Project Settings.

#include "EmberEnhancedPlayerInput.h"

#include "EnhancedActionKeyMapping.h"

FKey UEmberEnhancedPlayerInput::GetKeyForAction(UInputAction* InputAction) const
{
	const TArray<FEnhancedActionKeyMapping>& KeyMappings = GetEnhancedActionMappings();
	for (const FEnhancedActionKeyMapping& KeyMapping : KeyMappings)
	{
		if (KeyMapping.Action == InputAction)
			return KeyMapping.Key;
	}

	return FKey();
}
