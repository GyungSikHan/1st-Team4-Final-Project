// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AnimInstance/BaseAIAnimInstance.h"
#include "Crow_AnimInstance.generated.h"

UCLASS()
class EMBER_API UCrow_AnimInstance : public UBaseAIAnimInstance
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	float Crow_WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	float Crow_Height;
};
