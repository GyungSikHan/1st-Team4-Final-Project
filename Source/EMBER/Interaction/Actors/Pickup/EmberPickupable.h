// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EmberPickupable.generated.h"

class UItemInstance;

USTRUCT(BlueprintType)
struct FPickupInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UItemInstance> ItemInstance;
};

UINTERFACE()
class UEmberPickupable : public UInterface
{
	GENERATED_BODY()
};

class EMBER_API IEmberPickupable
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual FPickupInfo GetPickupInfo() const = 0;
};
