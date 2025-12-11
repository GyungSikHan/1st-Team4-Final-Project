// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "C_EmberGameMode.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API AC_EmberGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void RestartPlayer(AController* NewPlayer) override;
};
