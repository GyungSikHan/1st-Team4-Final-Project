// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTaskNode_IdleAnimation.h"

#include "CAIController.h"
#include "C_StateComponent.h"
#include "MontageSystemComponent.h"
#include "Base/BaseAI.h"

UBTTaskNode_IdleAnimation::UBTTaskNode_IdleAnimation()
{
	NodeName = "Idle Play Animation";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTaskNode_IdleAnimation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
	{
		UE_LOG(LogTemp, Error, L"controller is null");
		return EBTNodeResult::Failed;
	}
	ABaseAI* ai = Cast<ABaseAI>(controller->GetPawn());
	if(ai == nullptr)
	{
		UE_LOG(LogTemp, Error, L"ai is null");
		return EBTNodeResult::Failed;
	}
	UC_StateComponent* state = Cast<UC_StateComponent>(ai->GetComponentByClass(UC_StateComponent::StaticClass()));
	if (state == nullptr)
	{
		UE_LOG(LogTemp, Error, L"state component is null");
		return EBTNodeResult::Failed;
	}

	UMontageSystemComponent* montage = Cast<UMontageSystemComponent>(ai->GetComponentByClass(UMontageSystemComponent::StaticClass()));
	if(montage == nullptr)
	{
		UE_LOG(LogTemp, Error, L"montage is null");
		return EBTNodeResult::Failed;
	}
	state->SetAnimMode();
	controller->StopMovement();
	montage->PlayMontage(Montage);
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTaskNode_IdleAnimation::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
	{
		UE_LOG(LogTemp, Error, L"controller is null");
		return EBTNodeResult::Failed;
	}
	ABaseAI* ai = Cast<ABaseAI>(controller->GetPawn());
	if (ai == nullptr)
	{
		UE_LOG(LogTemp, Error, L"ai is null");
		return EBTNodeResult::Failed;
	}
	UC_StateComponent* state = Cast<UC_StateComponent>(ai->GetComponentByClass(UC_StateComponent::StaticClass()));
	if (state == nullptr)
	{
		UE_LOG(LogTemp, Error, L"state is null");
		return EBTNodeResult::Failed;
	}
	state->SetIdleMode();

	EndPlay(ai);

	return EBTNodeResult::Aborted;
}

void UBTTaskNode_IdleAnimation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
	{
		UE_LOG(LogTemp, Error, L"controller is null");
		return;
	}
	controller->StopMovement();
	ABaseAI* ai = Cast<ABaseAI>(controller->GetPawn());
	if (ai == nullptr)
	{
		UE_LOG(LogTemp, Error, L"ai is null");
		return;
	}
	UC_StateComponent* state = Cast<UC_StateComponent>(ai->GetComponentByClass(UC_StateComponent::StaticClass()));
	if (state == nullptr)
	{
		UE_LOG(LogTemp, Error, L"ai is null");
		return;
	}
	
	if (state->IsAnimMode() == false)
	{
		EndPlay(ai);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	if(IsMontageFinished(ai) == true)
	{
		EndPlay(ai);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}

ABaseAI* UBTTaskNode_IdleAnimation::GetAIFromOwnerComp(UBehaviorTreeComponent& OwnerComp)
{
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
		return nullptr;
	return Cast<ABaseAI>(controller->GetPawn());
}

bool UBTTaskNode_IdleAnimation::IsMontageFinished(ABaseAI* AI)
{
	if (AI == nullptr || Montage == nullptr)
		return true;

	UAnimInstance* animinstance = AI->GetMesh()->GetAnimInstance();
	if (animinstance == nullptr)
		return true;

	return animinstance->Montage_IsPlaying(Montage) == false;
}

void UBTTaskNode_IdleAnimation::EndPlay(ABaseAI* AI)
{
	UAnimInstance* animinstance = AI->GetMesh()->GetAnimInstance();
	animinstance->Montage_Stop(0.0f);
}
