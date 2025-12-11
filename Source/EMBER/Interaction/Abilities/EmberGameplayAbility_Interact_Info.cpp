// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberGameplayAbility_Interact_Info.h"

#include "Interaction/EmberInteractable.h"
#include "Interaction/EmberInteractionQuery.h"

class IEmberInteractable;

UEmberGameplayAbility_Interact_Info::UEmberGameplayAbility_Interact_Info(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UEmberGameplayAbility_Interact_Info::InitializeAbility(AActor* TargetActor)
{
	TScriptInterface<IEmberInteractable> TargetInteractable(TargetActor);

	if (TargetInteractable)
	{
		Interactable = TargetInteractable;
		InteractableActor = TargetActor;

		FEmberInteractionQuery InteractionQuery;
		InteractionQuery.RequestingAvatar = GetAvatarActorFromActorInfo();
		InteractionQuery.RequestingController = GetControllerFromActorInfo();
		
		InteractionInfo = TargetInteractable->GatherPostInteractionInfo(InteractionQuery, TargetInteractable);
		return true;
	}
	
	return false;
}
