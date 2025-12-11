// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Khaimera.generated.h"

/**
 * 
 */
UCLASS()
class EMBER_API UCBTService_Khaimera : public UBTService
{
	GENERATED_BODY()
public:
	UCBTService_Khaimera();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Action")
	float ActionRange = 150.0f;
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDrawDebug;
};
