// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTaskNode_Combo.h"

UBTTaskNode_Combo::UBTTaskNode_Combo()
{
}

EBTNodeResult::Type UBTTaskNode_Combo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

EBTNodeResult::Type UBTTaskNode_Combo::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTaskNode_Combo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}
