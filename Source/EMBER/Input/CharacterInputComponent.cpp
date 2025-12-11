// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterInputComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EmberEnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFlag.h"
#include "UI/Data/EmberPawnData.h"
#include "Data/EmberInputConfig.h"
#include "GameFramework/PlayerState.h"
#include "GameInfo/GameplayTags.h"
#include "System/AbilitySystem/EmberAbilitySystemComponent.h"

UCharacterInputComponent::UCharacterInputComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCharacterInputComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	
	if (const UEmberInputConfig* InputConfig =  UEmberPawnData::Get().InputConfig)
	{
		Subsystem->AddMappingContext(InputConfig->InputMappingContext, 1);

		if (UEmberEnhancedInputComponent* EmberIC = Cast<UEmberEnhancedInputComponent>(PlayerInputComponent))
		{
			TArray<uint32> BindHandles;
			EmberIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagStarted, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

			EmberIC->BindNativeAction(InputConfig, EmberGameplayTags::InputTag_QuickSlot_Weapon_Primary, ETriggerEvent::Triggered, this, &ThisClass::InputTag_QuickSlot_Weapon_Primary);
			EmberIC->BindNativeAction(InputConfig, EmberGameplayTags::InputTag_QuickSlot_Utility_Primary, ETriggerEvent::Triggered, this, &ThisClass::InputTag_QuickSlot_Utility_Primary);
			EmberIC->BindNativeAction(InputConfig, EmberGameplayTags::InputTag_QuickSlot_Utility_Secondary, ETriggerEvent::Triggered, this, &ThisClass::InputTag_QuickSlot_Utility_Secondary);
			EmberIC->BindNativeAction(InputConfig, EmberGameplayTags::InputTag_QuickSlot_Utility_Tertiary, ETriggerEvent::Triggered, this, &ThisClass::InputTag_QuickSlot_Utility_Tertiary);
			EmberIC->BindNativeAction(InputConfig, EmberGameplayTags::InputTag_QuickSlot_Utility_Quaternary, ETriggerEvent::Triggered, this, &ThisClass::InputTag_QuickSlot_Utility_Quaternary);
		}
	}
}

void UCharacterInputComponent::Input_AbilityInputTagStarted(FGameplayTag InputTag)
{
	if (const APlayerState* PlayerState = GetPlayerState<APlayerState>())
	{
		if (UEmberAbilitySystemComponent* EmberSC = PlayerState->FindComponentByClass<UEmberAbilitySystemComponent>())
		{
				EmberSC->AbilityInputTagStarted(InputTag);
		}
	}
}

void UCharacterInputComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APlayerState* PlayerState = GetPlayerState<APlayerState>())
	{
		if (UEmberAbilitySystemComponent* EmberSC = PlayerState->FindComponentByClass<UEmberAbilitySystemComponent>())
		{
			EmberSC->AbilityInputTagPressed(InputTag);
		}
	}
}

void UCharacterInputComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (const APlayerState* PlayerState = GetPlayerState<APlayerState>())
	{
		if (UEmberAbilitySystemComponent* EmberSC = PlayerState->FindComponentByClass<UEmberAbilitySystemComponent>())
		{
			EmberSC->AbilityInputTagReleased(InputTag);
		}
	}
}

void UCharacterInputComponent::InputTag_QuickSlot_Weapon_Primary()
{
	FGameplayEventData Payload;
	Payload.EventMagnitude = (int32)EEquipState::Weapon_Primary;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), EmberGameplayTags::GameplayEvent_QuickSlot_Handle, Payload);
}

void UCharacterInputComponent::InputTag_QuickSlot_Utility_Primary()
{
	FGameplayEventData Payload;
	Payload.EventMagnitude = (int32)EEquipState::Utility_Primary;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), EmberGameplayTags::GameplayEvent_QuickSlot_Handle, Payload);
}

void UCharacterInputComponent::InputTag_QuickSlot_Utility_Secondary()
{
	FGameplayEventData Payload;
	Payload.EventMagnitude = (int32)EEquipState::Utility_Secondary;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), EmberGameplayTags::GameplayEvent_QuickSlot_Handle, Payload);

}

void UCharacterInputComponent::InputTag_QuickSlot_Utility_Tertiary()
{
	FGameplayEventData Payload;
	Payload.EventMagnitude = (int32)EEquipState::Utility_Tertiary;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), EmberGameplayTags::GameplayEvent_QuickSlot_Handle, Payload);
}

void UCharacterInputComponent::InputTag_QuickSlot_Utility_Quaternary()
{
	FGameplayEventData Payload;
	Payload.EventMagnitude = (int32)EEquipState::Utility_Quaternary;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), EmberGameplayTags::GameplayEvent_QuickSlot_Handle, Payload);
}
