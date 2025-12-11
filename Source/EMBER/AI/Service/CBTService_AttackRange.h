// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_AttackRange.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UCBTService_AttackRange : public UBTService
{
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif
};
