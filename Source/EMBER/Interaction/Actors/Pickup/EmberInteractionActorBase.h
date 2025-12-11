// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/Actors/EmberWorldPickupable.h"
#include "EmberInteractionActorBase.generated.h"

class UItemTemplate;
class USphereComponent;

UCLASS()
class EMBER_API AEmberInteractionActorBase : public AEmberWorldPickupable
{
	GENERATED_BODY()

public:
	AEmberInteractionActorBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(EditDefaultsOnly, Category = "PickupableItemBase|Info")
	TSubclassOf<UItemTemplate> ItemTemplateClass;
};
