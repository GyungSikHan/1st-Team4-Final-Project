// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTT_StopMovement.h"

#include "CAIController.h"
#include "AnimInstance/BaseAIAnimInstance.h"
#include "Base/BaseAI.h"


EBTNodeResult::Type UBTT_StopMovement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	ACAIController* AIController = Cast<ACAIController>(OwnerComp.GetOwner());
	ABaseAI* AI = Cast<ABaseAI>(AIController->GetPawn());
	UBaseAIAnimInstance* AnimInstance = Cast<UBaseAIAnimInstance>(AI->GetMesh()->GetAnimInstance());

	AIController->StopMovement();
	AnimInstance->StopAllMontages(0.5f);
	return EBTNodeResult::Succeeded;
}
