// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_IdleAnimation.generated.h"

class ABaseAI;
/**
 * 
 */
UCLASS()
class EMBER_API UBTTaskNode_IdleAnimation : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTaskNode_IdleAnimation();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	ABaseAI* GetAIFromOwnerComp(UBehaviorTreeComponent& OwnerComp);
	bool IsMontageFinished(ABaseAI* AI);
	void EndPlay(ABaseAI* AI);
private:
	UPROPERTY(EditAnywhere)
	UAnimMontage* Montage;
};


