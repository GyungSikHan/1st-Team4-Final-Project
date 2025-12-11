// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Base/AggressiveAI.h"

AAggressiveAI::AAggressiveAI()
{
	//AnimalType = EAnimalType::Aggressive;
}

void AAggressiveAI::BeginPlay()
{
	Super::BeginPlay();
}

float AAggressiveAI::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0 && !bIsDie)
	{
		// SetRunSpeed();
	}

	return ActualDamage;
}

//void AAggressiveAI::OnTargetPerceptionUpdated(AActor* UpdatedActor, FAIStimulus Stimulus)
//{
//	Super::OnTargetPerceptionUpdated(UpdatedActor, Stimulus);
//
//	if (Stimulus.WasSuccessfullySensed())
//	{
//		SetRunSpeed();
//	}
//	else
//	{
//		SetWalkSpeed();
//	}
//}
