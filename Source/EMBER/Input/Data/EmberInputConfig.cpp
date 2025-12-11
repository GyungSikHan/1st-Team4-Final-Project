// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberInputConfig.h"

#include "System/EmberAssetManager.h"

UEmberInputConfig::UEmberInputConfig(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

const UInputAction* UEmberInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FEmberInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	return nullptr;
}

const UInputAction* UEmberInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FEmberInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	return nullptr;
}
