// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Notify/CAnimNotify_AISound.h"

#include "CAIController.h"
#include "Base/BaseAI.h"

FString UCAnimNotify_AISound::GetNotifyName_Implementation() const
{
	return "AI Sound";
}

void UCAnimNotify_AISound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp == nullptr)
	{
		UE_LOG(LogTemp, Error, L"AI Sound Notify, MeshComp is null");
		return;
	}
	TObjectPtr<ABaseAI> ai = Cast<ABaseAI>(MeshComp->GetOwner());
	if (ai == nullptr)
	{
		UE_LOG(LogTemp, Error, L"AI Sound Notify, AI is null");
		return;
	}
	ai->PlaySound(SoundType);
}
