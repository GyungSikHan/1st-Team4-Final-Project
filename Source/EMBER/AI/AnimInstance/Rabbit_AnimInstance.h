// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AnimInstance/BaseAIAnimInstance.h"
#include "Rabbit_AnimInstance.generated.h"

UCLASS()
class EMBER_API URabbit_AnimInstance : public UBaseAIAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
};
