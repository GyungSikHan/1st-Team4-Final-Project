// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Animal/Rabbit.h"

#include "AI/NavigationSystemBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ARabbit::ARabbit()
{
	// MaxHP = 100;
}

void ARabbit::OnDeath()
{
	Super::OnDeath();
	UE_LOG(LogTemp, Warning, TEXT("Rabbit Death"));

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this,
	                                       &ARabbit::Cleaning,
	                                       5.0f, false);
}

void ARabbit::Cleaning()
{
	Destroy();
}
