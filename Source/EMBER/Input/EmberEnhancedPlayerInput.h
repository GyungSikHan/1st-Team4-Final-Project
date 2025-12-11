// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedPlayerInput.h"
#include "EmberEnhancedPlayerInput.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UEmberEnhancedPlayerInput : public UEnhancedPlayerInput
{
	GENERATED_BODY()

public:
	FKey GetKeyForAction(UInputAction* InputAction) const;
};
