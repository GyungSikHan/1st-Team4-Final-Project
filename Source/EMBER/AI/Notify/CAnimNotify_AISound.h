// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_AISound.generated.h"

enum class AISoundCategory : uint8;

UCLASS()
class EMBER_API UCAnimNotify_AISound : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere, Category="Sound")
	AISoundCategory SoundType;
};
