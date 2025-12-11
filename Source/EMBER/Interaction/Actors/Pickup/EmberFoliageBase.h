// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Interaction/Actors/EmberWorldPickupable.h"
#include "EmberFoliageBase.generated.h"

class AEmberInteractionActorBase;
class UItemTemplate;

UCLASS()
class EMBER_API AEmberFoliageBase : public AEmberWorldPickupable
{
	GENERATED_BODY()
	
public:
	AEmberFoliageBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "PickupableItemBase|Info")
	TSubclassOf<UItemTemplate> ItemTemplateClass;
};
