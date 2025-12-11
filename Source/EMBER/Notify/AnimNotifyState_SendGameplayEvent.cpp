// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_SendGameplayEvent.h"

#include "AbilitySystemBlueprintLibrary.h"

UAnimNotifyState_SendGameplayEvent::UAnimNotifyState_SendGameplayEvent(const FObjectInitializer& ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	// 노티파이가 애니메이션 에디터에서 프리뷰 중에는 실행되지 않도록 설정
	bShouldFireInEditor = false;
#endif
	// 해당 프레임에 정확하게 함수가 호출되도록 설정
	bIsNativeBranchingPoint = true;
}

void UAnimNotifyState_SendGameplayEvent::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (BeginEventTag.IsValid())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), BeginEventTag, EventData);
	}
}

void UAnimNotifyState_SendGameplayEvent::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (EndEventTag.IsValid())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EndEventTag, EventData);
	}
}