// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Dragon.generated.h"

UCLASS()
class EMBER_API UCBTService_Dragon : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_Dragon();

	float GetActionRange() const { return ActionRange; }
	float GetMeleeRange() const { return MeleeRange; }

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Action")
	float ActionRange = 1500.f;

	UPROPERTY(EditAnywhere, Category = "Action")
	float MeleeRange = 1000.f;

	UPROPERTY()
	ACharacter* CachedTarget = nullptr;

	UPROPERTY(EditAnywhere)
	float TargetMemoryTime;

	float LastSeenTime;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDrawDebug;
};
