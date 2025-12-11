// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTT_Hitte.h"
#include "CAIController.h"
#include "C_StateComponent.h"
#include "Base/BaseAI.h"

UBTT_Hitte::UBTT_Hitte()
{
	NodeName = L"Hitted";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_Hitte::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
	{
		UE_LOG(LogTemp, Error, L"task Controller is null");
		return EBTNodeResult::Failed;
	}
	ABaseAI* ai = Cast<ABaseAI>(controller->GetPawn());
	if (ai == nullptr)
	{
		UE_LOG(LogTemp, Error, L"task ai is null");
		return EBTNodeResult::Failed;
	}
	UC_StateComponent* state = Cast<UC_StateComponent>(ai->GetComponentByClass(UC_StateComponent::StaticClass()));
	if (state == nullptr)
	{
		UE_LOG(LogTemp, Error, L"task state is null");
		return EBTNodeResult::Failed;
	}
	if(state->IsHittdMode() == false)
	{
		UE_LOG(LogTemp, Error, L"Not Hitted  Mode");
		return EBTNodeResult::Failed;
	}

	controller->StopMovement();
	
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTT_Hitte::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTT_Hitte::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
	{
		UE_LOG(LogTemp, Error, L"task Controller is null");
		return;
	}
	ABaseAI* ai = Cast<ABaseAI>(controller->GetPawn());
	if (ai == nullptr)
	{
		UE_LOG(LogTemp, Error, L"task ai is null");
		return;
	}
	UC_StateComponent* state = Cast<UC_StateComponent>(ai->GetComponentByClass(UC_StateComponent::StaticClass()));
	if (state == nullptr)
	{
		UE_LOG(LogTemp, Error, L"task state is null");
		return;
	}
	if (state->IsHittdMode() == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}
