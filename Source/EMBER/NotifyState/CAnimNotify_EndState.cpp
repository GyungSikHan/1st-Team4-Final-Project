// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyState/CAnimNotify_EndState.h"

#include "C_StateComponent.h"
#include "EmberPlayerCharacter.h"
#include "Base/BaseAI.h"
#include "GameFramework/Character.h"

FString UCAnimNotify_EndState::GetNotifyName_Implementation() const
{
	return "End State";
}

void UCAnimNotify_EndState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if(MeshComp == nullptr)
	{
		UE_LOG(LogTemp, Error, L"End State, MeshComp is null");
		return;
	}
	if(MeshComp->GetOwner() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"End State, owner is null");
		return;
	}
	AEmberPlayerCharacter* player = Cast<AEmberPlayerCharacter>(MeshComp->GetOwner());
	ABaseAI* ai;
	UC_StateComponent* state;

	if(player != nullptr)
		state = Cast<UC_StateComponent>(player->GetComponentByClass(UC_StateComponent::StaticClass()));
	else
	{
		ai = Cast<ABaseAI>(MeshComp->GetOwner());
		if(ai == nullptr)
		{
			UE_LOG(LogTemp, Error, L"End State, character is null");
			return;
		}
		state = Cast<UC_StateComponent>(ai->GetComponentByClass(UC_StateComponent::StaticClass()));
	}
	
	if(state == nullptr)
	{
		UE_LOG(LogTemp, Error, L"End State, state is null");
		return;
	}

	//if (state->IsIdleMode() == false)
	state->SetIdleMode();
}
