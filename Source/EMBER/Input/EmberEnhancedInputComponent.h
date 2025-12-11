// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Data/EmberInputConfig.h"
#include "EnhancedInputComponent.h"
#include "EmberEnhancedInputComponent.generated.h"


UCLASS(Config = Input)
class EMBER_API UEmberEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	UEmberEnhancedInputComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	template<class UserObject,typename CallbackFunc>
	void BindNativeAction(const UEmberInputConfig* InInputConfig,const FGameplayTag& InInputTag,ETriggerEvent TriggerEvent,UserObject* ContextObject,CallbackFunc Func);

	template<class UserClass, typename StartedFuncType, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UEmberInputConfig* InputConfig, UserClass* Object, StartedFuncType StartedFunc, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);
};

template<class UserObject, typename CallbackFunc>
void UEmberEnhancedInputComponent::BindNativeAction(const UEmberInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag))
	{
		BindAction(IA,TriggerEvent,ContextObject,Func);
	}
}

template <class UserClass, typename StartedFuncType, typename PressedFuncType, typename ReleasedFuncType>
void UEmberEnhancedInputComponent::BindAbilityActions(const UEmberInputConfig* InputConfig, UserClass* Object, StartedFuncType StartedFunc, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);

	for (const FEmberInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (StartedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Started, Object, StartedFunc, Action.InputTag).GetHandle());
			}
		
			if (PressedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle());
			}

			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle());
			}
		}
	}
}
