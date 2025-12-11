#include "BTT_Run.h"
#include "AI/Base/BaseAI.h"
#include "CAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_Run::UBTT_Run()
{
	NodeName = "RunState";
	Runaway = 1500.0f;
}

EBTNodeResult::Type UBTT_Run::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerCompRef = &OwnerComp;
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetAIOwner());
	BlackboardComponent = OwnerComp.GetBlackboardComponent();
	BaseAI = Cast<ABaseAI>(Controller->GetPawn());
	AActor* Target = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActor"));

	AIState = Cast<UC_StateComponent>(BaseAI->GetComponentByClass(UC_StateComponent::StaticClass()));

	if (Target == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("RunTarget is null"));
		return EBTNodeResult::InProgress;
	}
	Controller->StopMovement();
	BaseAI->GetMesh()->GetAnimInstance()->StopAllMontages(1.0f);

	FVector TargetLocation = Target->GetActorLocation();
	FVector AI_Location = BaseAI->GetActorLocation();
	FVector Direction = (AI_Location - TargetLocation).GetSafeNormal(); //방향벡터만 남기고 1로 설정
	FVector NewLocation = AI_Location + Direction * Runaway;

	// ExecuteTask 안에서:
	Controller->ReceiveMoveCompleted.RemoveAll(this);
	QuestID = Controller->MoveToLocation(NewLocation, 50.f);
	if (!QuestID.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("MoveToLocation 실패"));
		return EBTNodeResult::Failed;
	}
	Controller->ReceiveMoveCompleted.AddDynamic(this, &UBTT_Run::OnMoveCompleted);

	return EBTNodeResult::InProgress;
}

void UBTT_Run::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	// UE_LOG(LogTemp, Error, TEXT("OnMoveCompleted 호출"));
	// UE_LOG(LogTemp, Warning, TEXT("Expected QuestID: %d, Actual RequestID: %d"), QuestID.GetID(), RequestID.GetID());
	//
	// UE_LOG(LogTemp, Warning, TEXT("Invalid MoveComplete"));
	AIState->SetIdleMode();
	BlackboardComponent->SetValueAsObject("TargetActor", nullptr);
	FinishLatentTask(*OwnerCompRef, EBTNodeResult::Succeeded);
}
