// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Base/BaseAI.h"
#include "AggressiveAI.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API AAggressiveAI : public ABaseAI
{
	GENERATED_BODY()
	
public:
	AAggressiveAI();
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
//	virtual void OnTargetPerceptionUpdated(AActor* UpdatedActor, FAIStimulus Stimulus) override;
};