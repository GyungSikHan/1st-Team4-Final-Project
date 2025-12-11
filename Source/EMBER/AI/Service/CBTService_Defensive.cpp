// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/CBTService_Defensive.h"
#include "AI/BehaviorTree/CBehaviorTreeComponent.h"
#include "CAIController.h"
#include "C_StateComponent.h"
#include "AI/Base/HumanAIBase.h"

UCBTService_Defensive::UCBTService_Defensive()
{
	NodeName = "Defensive";
	Interval = 1.0f;
	RandomDeviation = 0.0f;
}

void UCBTService_Defensive::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	TObjectPtr<ACAIController> controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller.Get() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"controller is null");
		return;
	}
	TObjectPtr<ABaseAI> ai = Cast<ABaseAI>(controller->GetPawn());
	if (ai.Get() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"ai is null");
		return;
	}
	TObjectPtr<UC_StateComponent> state = Cast<UC_StateComponent>(ai->GetComponentByClass(UC_StateComponent::StaticClass()));
	if (state.Get() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"state is null");
		return;
	}
	TObjectPtr<UCBehaviorTreeComponent> aiState = Cast<UCBehaviorTreeComponent>(ai->GetComponentByClass(UCBehaviorTreeComponent::StaticClass()));
	if (aiState.Get() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"aistate is null");
		return;
	}

	if (bDrawDebug)
	{
		FVector start = ai->GetActorLocation();
		start.Z -= 25;

		FVector end = start;

		DrawDebugCylinder(ai->GetWorld(), start, end, ActionRange, 10, FColor::Red, false, Interval);
	}

	if (state->IsDeadMode() == true)
	{
		//controller->StopMovement();
		aiState->SetDeadMode();
		return;
	}

	if (state->IsHittdMode() == true && bisHit == false)
	{
		aiState->SetHittedMode();
		aiState->SetTarget(ai->GetDamagesData().Character);
		bisHit = true;
		return;
	}

	TObjectPtr<ACharacter> target;
	if(bisHit == true)
		target = aiState->GetTarget();
	if (target == nullptr)
	{
		bisHit = false;
		aiState->SetPatrolMode();
		return;
	}

	float distance = ai.Get()->GetDistanceTo(target);
	if (distance < ActionRange)
	{
		aiState->SetActionMode();
		return;
	}

	aiState->SetChaseMode();
}
