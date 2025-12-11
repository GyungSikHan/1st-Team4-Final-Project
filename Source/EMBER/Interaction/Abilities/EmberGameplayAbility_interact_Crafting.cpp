// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberGameplayAbility_interact_Crafting.h"

#include "EmberPlayerCharacter.h"
#include "EmberPlayerController.h"
#include "Actions/AsyncAction_PushContentToLayerForPlayer.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameInfo/GameplayTags.h"
#include "Interaction/Actors/Crafting/EternalFlame.h"
#include "UI/Inventory/InventorySlotsWidget.h"
#include "Item/Managers/InventoryManagerComponent.h"
#include "Item/Managers/InventoryEquipmentManagerComponent.h"
#include "UI/Crafting/CraftingWidget.h"
#include "UI/Inventory/Equipment/EquipmentSlotsWidget.h"

UEmberGameplayAbility_interact_Crafting::UEmberGameplayAbility_interact_Crafting(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UEmberGameplayAbility_interact_Crafting::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (TriggerEventData == nullptr || bInitialized == false)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	if (IsLocallyControlled())
	{
		if (UAsyncAction_PushContentToLayerForPlayer* PushWidgetAction = UAsyncAction_PushContentToLayerForPlayer::PushContentToLayerForPlayer(GetEmberPlayerControllerFromActorInfo(), WidgetClass, WidgetLayerTag, true))
		{
			PushWidgetAction->AfterPush.AddDynamic(this, &ThisClass::OnAfterPushWidget);
			PushWidgetAction->Activate();
		}
	}
}

void UEmberGameplayAbility_interact_Crafting::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEmberGameplayAbility_interact_Crafting::OnAfterPushWidget(UCommonActivatableWidget* InPushedWidget)
{
	PushedWidget = InPushedWidget;
	
	AEmberPlayerCharacter* EmberCharacter = GetEmberCharacterFromActorInfo();
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	
	FInventoryInitializeMessage MyInventoryInitMessage;
	MyInventoryInitMessage.InventoryManager = EmberCharacter->GetComponentByClass<UInventoryManagerComponent>();
	MessageSubsystem.BroadcastMessage(EmberGameplayTags::Message_Initialize_MyInventory, MyInventoryInitMessage);
	
	FEquipmentInitializeMessage MyEquipmentInitMessage;
	MyEquipmentInitMessage.InventoryEquipmentManager = EmberCharacter->GetComponentByClass<UInventoryEquipmentManagerComponent>();
	MessageSubsystem.BroadcastMessage(EmberGameplayTags::Message_Initialize_InventoryEquipment, MyEquipmentInitMessage);

	FInventoryInitializeMessage OtherInventoryInitMessage;
	OtherInventoryInitMessage.InventoryManager = InteractableActor->GetComponentByClass<UInventoryManagerComponent>();
	MessageSubsystem.BroadcastMessage(EmberGameplayTags::Message_Initialize_OtherInventory, OtherInventoryInitMessage);

	FCraftingWidgetInitializeMessage MyCraftingInitMessage;
	MyCraftingInitMessage.CraftingBuilding = Cast<AEternalFlame>(InteractableActor);;
	MessageSubsystem.BroadcastMessage(EmberGameplayTags::Message_Initialize_CraftingWidget, MyCraftingInitMessage);

	InPushedWidget->OnDeactivated().AddWeakLambda(this, [this]()
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	});
}
