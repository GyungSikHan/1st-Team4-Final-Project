// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberGameplayAbility_Interact.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameInfo/GameplayTags.h"
#include "Interaction/EmberCollisionChannel.h"
#include "Interaction/EmberInteractionQuery.h"
#include "Interaction/Tasks/EmberAbilityTask_WaitForInteractableTraceHit.h"
#include "Interaction/Tasks/EmberAbilityTask_WaitInputStart.h"

UEmberGameplayAbility_Interact::UEmberGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ActivationPolicy = EEmberAbilityActivationPolicy::OnSpawn;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;
}

void UEmberGameplayAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FEmberInteractionQuery InteractionQuery;
	InteractionQuery.RequestingAvatar = GetAvatarActorFromActorInfo();
	InteractionQuery.RequestingController = GetControllerFromActorInfo();

	if (UEmberAbilityTask_WaitForInteractableTraceHit* TraceHitTask = UEmberAbilityTask_WaitForInteractableTraceHit::WaitForInteractableTraceHit(this, InteractionQuery, Ember_TraceChannel_Interaction, MakeTargetLocationInfoFromOwnerActor(), InteractionTraceRange, InteractionTraceRate, bShowTraceDebug))
	{
		TraceHitTask->InteractableChanged.AddDynamic(this, &ThisClass::UpdateInteractions);
		TraceHitTask->ReadyForActivation();
	}

	WaitInputStart();
}

void UEmberGameplayAbility_Interact::UpdateInteractions(const FEmberInteractionInfo& InteractionInfo)
{
	AActor* InteractableActor = Cast<AActor>(InteractionInfo.Interactable.GetObject());
	FString ActorName = IsValid(InteractableActor) ? InteractableActor->GetName() : TEXT("None");
	UE_LOG(LogTemp, Warning, TEXT("[DEBUG 1] Scanner found interactable: %s"), *ActorName);
	
	FEmberInteractionMessage Message;
	Message.Instigator = GetAvatarActorFromActorInfo();
	Message.bShouldRefresh = true;
	Message.bSwitchActive = (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(EmberGameplayTags::Status_Interact) == false);
	Message.InteractionInfo = InteractionInfo;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetAvatarActorFromActorInfo());
	MessageSystem.BroadcastMessage(EmberGameplayTags::Message_Interaction_Notice, Message);

	CurrentInteractionInfo = InteractionInfo;
}

void UEmberGameplayAbility_Interact::TriggerInteraction()
{
	if (CurrentInteractionInfo.Interactable == nullptr)
		return;
	
	if (GetAbilitySystemComponentFromActorInfo())
	{
		UE_LOG(LogTemp, Warning, TEXT("[DEBUG 2] F-Key pressed! Trying to trigger interaction..."));
		
		if (CurrentInteractionInfo.Interactable == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("[DEBUG 2] FAILED: CurrentInteractionInfo is NULL!"));
			return;
		}
		
		AActor* Instigator = GetAvatarActorFromActorInfo();
		AActor* InteractableActor = Cast<AActor>(CurrentInteractionInfo.Interactable.GetObject());

		FGameplayEventData Payload;
		Payload.EventTag = EmberGameplayTags::Ability_Interact_Active;
		Payload.Instigator = Instigator;
		Payload.Target = InteractableActor;
		
		SendGameplayEvent(EmberGameplayTags::Ability_Interact_Active, Payload);
	}
}

void UEmberGameplayAbility_Interact::WaitInputStart()
{
	bool test = HasAuthority(&CurrentActivationInfo);
	
	if (UEmberAbilityTask_WaitInputStart* InputStartTask = UEmberAbilityTask_WaitInputStart::WaitInputStart(this))
	{
		InputStartTask->OnStart.AddDynamic(this, &ThisClass::OnInputStart);
		InputStartTask->ReadyForActivation();
	}
}

void UEmberGameplayAbility_Interact::OnInputStart()
{
	TriggerInteraction();
	WaitInputStart();
}
