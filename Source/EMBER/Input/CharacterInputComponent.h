// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "CharacterInputComponent.generated.h"

struct FGameplayTag;
/**
 * 
 */
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class EMBER_API UCharacterInputComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UCharacterInputComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	void InitializePlayerInput(UInputComponent* PlayerInputComponent);

protected:
	void Input_AbilityInputTagStarted(FGameplayTag InputTag);
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	void InputTag_QuickSlot_Weapon_Primary();
	void InputTag_QuickSlot_Utility_Primary();
	void InputTag_QuickSlot_Utility_Secondary();
	void InputTag_QuickSlot_Utility_Tertiary();
	void InputTag_QuickSlot_Utility_Quaternary();
};
