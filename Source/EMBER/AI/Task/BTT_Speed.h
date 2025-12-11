// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFlag.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Speed.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UBTT_Speed : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_Speed();

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere)
	ESpeedType SpeedType;
};
