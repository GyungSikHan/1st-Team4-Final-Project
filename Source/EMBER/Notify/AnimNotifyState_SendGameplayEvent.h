// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_SendGameplayEvent.generated.h"

UCLASS(meta=(DisplayName="Send Gameplay Event State"))
class EMBER_API UAnimNotifyState_SendGameplayEvent : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAnimNotifyState_SendGameplayEvent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~UAnimNotifyState Overrides
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	//~End of UAnimNotifyState Overrides
	
protected:
	UPROPERTY(EditAnywhere)
	FGameplayTag BeginEventTag;

	UPROPERTY(EditAnywhere)
	FGameplayTag EndEventTag;

	UPROPERTY(EditAnywhere)
	FGameplayEventData EventData;
};
