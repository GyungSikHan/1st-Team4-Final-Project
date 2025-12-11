// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/CAnimNotify_Dead.h"

#include "EmberPlayerCharacter.h"
#include "AIComponent/CAIWeaponComponent.h"
#include "Base/BaseAI.h"
#include "GameFramework/Character.h"

void UCAnimNotify_Dead::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                               const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Mesh Component is null");
		return;
	}
	if (MeshComp->GetOwner() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Owner Actor is null");
		return;
	}

	ABaseAI* ai = Cast<ABaseAI>(MeshComp->GetOwner());
	if(ai != nullptr)
	{
		ai->EndDeath();
		return;
	}
	AEmberPlayerCharacter* player = Cast<AEmberPlayerCharacter>(MeshComp->GetOwner());
	if(player != nullptr)
		player->EndDeath();
}

FString UCAnimNotify_Dead::GetNotifyName_Implementation() const
{
	return "Dead";
}
