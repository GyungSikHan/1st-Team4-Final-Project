// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberPlayerController.h"

#include "EmberPlayerCharacter.h"
#include "EmberPlayerState.h"
#include "EnhancedInputSubsystems.h"
#include "ItemTemplate.h"
#include "ItemInstance.h"
#include "Managers/EquipmentManagerComponent.h"
#include "System/AbilitySystem/EmberAbilitySystemComponent.h"
#include "UI/Data/EmberItemData.h"

class AEmberPlayerState;

AEmberPlayerController::AEmberPlayerController()
	: InputMappingContext(nullptr),
	  MoveAction(nullptr),
	  LookAction(nullptr),
	  SprintAction(nullptr),
      JumpAction(nullptr)
{
}

void AEmberPlayerController::BeginPlay()
{
	Super::BeginPlay();
 
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}

void AEmberPlayerController::Server_EquipWeapon_Implementation(EWeaponSlotType WeaponSlotType, TSubclassOf<UItemTemplate> ItemTemplateClass)
{
	AEmberPlayerCharacter* EmberCharacter = Cast<AEmberPlayerCharacter>(GetPawn());
	if (EmberCharacter == nullptr)
		return;

	UEquipmentManagerComponent* EquipmentManager = EmberCharacter->FindComponentByClass<UEquipmentManagerComponent>();
	if (EquipmentManager == nullptr)
		return;

	EquipmentManager->AddEquipment(WeaponSlotType, ItemTemplateClass);
}

void AEmberPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UEmberAbilitySystemComponent* EmberASC = GetEmberAbilitySystemComponent())
	{
		EmberASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

UEmberAbilitySystemComponent* AEmberPlayerController::GetEmberAbilitySystemComponent() const
{
	const AEmberPlayerState* EmberPS = CastChecked<AEmberPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
	return (EmberPS ? EmberPS->GetEmberAbilitySystemComponent() : nullptr);
}
