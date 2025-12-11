// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FEmberBodyTemperatureQuery.generated.h"

USTRUCT(BlueprintType)
struct EMBER_API FEmberBodyTemperatureQuery
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AActor> RequestingAvatar;
	
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AController> RequestingController;
};