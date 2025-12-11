// Fill out your copyright notice in the Description page of Project Settings.


#include "EmberInteractionActorBase.h"

#include "Components/SphereComponent.h"

AEmberInteractionActorBase::AEmberInteractionActorBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bReplicates = true;
	
	InteractionInfo.bVisible = true;
	PrimaryActorTick.bCanEverTick = false;
}