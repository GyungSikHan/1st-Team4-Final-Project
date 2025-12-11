// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/CBTService_AttackRange.h"

#include "CAIController.h"
#include "Base/BaseAI.h"

void UCBTService_AttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
}
#if WITH_EDITOR
FName UCBTService_AttackRange::GetNodeIconName() const
{
	return "AttackRange";
}
#endif
