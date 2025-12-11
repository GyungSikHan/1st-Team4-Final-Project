// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_Random.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UBTDecorator_Random : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_Random();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
