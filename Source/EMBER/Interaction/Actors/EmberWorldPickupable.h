// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmberWorldInteractable.h"
#include "GameFramework/Actor.h"
#include "Interaction/EmberInteractable.h"
#include "Pickup/EmberPickupable.h"
#include "EmberWorldPickupable.generated.h"

class UItemInstance;

UCLASS(Abstract, Blueprintable)
class EMBER_API AEmberWorldPickupable : public AEmberWorldInteractable, public IEmberPickupable
{
	GENERATED_BODY()

public:
	AEmberWorldPickupable(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	//~IEmberInteractable Override
	virtual FEmberInteractionInfo GetPreInteractionInfo(const FEmberInteractionQuery& InteractionQuery) const override;
	virtual UMeshComponent* GetMeshComponent() const override;
	//~End of IEmberInteractable Override
	
public:
	//~IEmberPickupable Override
	UFUNCTION()
	virtual FPickupInfo GetPickupInfo() const override { return PickupInfo; }
	//~End of IEmberPickupable Override

public:
	virtual void SetPickupInfo(const FPickupInfo& InPickupInfo);
	
protected:
	UFUNCTION()
	virtual void OnRep_PickupInfo();

protected:
	UPROPERTY(ReplicatedUsing=OnRep_PickupInfo)
	FPickupInfo PickupInfo;
};
