// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFlag.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_FlySpeed.generated.h"




UCLASS()
class EMBER_API UBTT_FlySpeed : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	EFlySpeedType FlySpeedType;
};
