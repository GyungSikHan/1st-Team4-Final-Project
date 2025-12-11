#include "BTT_Patrol.h"
#include "AI/Base/BaseAI.h"
#include "CAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_Patrol::UBTT_Patrol()
{
	NodeName = "PatrolState";
}

EBTNodeResult::Type UBTT_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerCompRef = &OwnerComp;
	ACAIController* BaseAIController = Cast<ACAIController>(OwnerComp.GetAIOwner());
	if (!BaseAIController)
	{
		UE_LOG(LogTemp, Error, TEXT("BaseAIController is nullptr!"));
		FinishLatentTask(*OwnerCompRef, EBTNodeResult::Failed);
	}

	BlackboardComp = OwnerComp.GetBlackboardComponent(); //블랙보드 참조
	ABaseAI* ControlledAnimal = Cast<ABaseAI>(BaseAIController->GetPawn()); //사용되는 액터 참조

	if (ControlledAnimal->PatrolPoint.IsEmpty() || ControlledAnimal->PatrolPoint.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("empty PatrolPoint"));
		BlackboardComp->SetValueAsBool("IsRest", true);
		FinishLatentTask(*OwnerCompRef, EBTNodeResult::Failed);
	}

	int32 CurrentIndex = BlackboardComp->GetValueAsInt("PatrolIndex"); //현재 위치인덱스
	CurrentIndex = (CurrentIndex + 1) % ControlledAnimal->PatrolPoint.Num(); //다음이동인덱스 업데이트

	ATargetPoint* NextPoint = ControlledAnimal->PatrolPoint[CurrentIndex]; //이동할액터위치 설정
	BlackboardComp->SetValueAsInt("PatrolIndex", CurrentIndex); //블랙보드에 인덱스 업데이트

	BaseAIController->ReceiveMoveCompleted.RemoveAll(this);
	BaseAIController->ReceiveMoveCompleted.AddDynamic(this, &UBTT_Patrol::OnMoveCompleted);
	PatrolRequestID = BaseAIController->MoveToActor(NextPoint, 100.0f);


	// BaseAIController->MoveToActor(NextPoint, 50.0f); // AcceptanceRadius 조정
	// BaseAIController->ReceiveMoveCompleted.RemoveDynamic(this, &UBTT_Patrol::OnMoveCompleted);
	// BaseAIController->ReceiveMoveCompleted.AddDynamic(this, &UBTT_Patrol::OnMoveCompleted);

	return EBTNodeResult::InProgress;
}

void UBTT_Patrol::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (PatrolRequestID == RequestID)
	{
		if (Result == EPathFollowingResult::Success)
		{
			UE_LOG(LogTemp, Warning, TEXT("PatrolCompleted"));
			BlackboardComp->SetValueAsBool("IsRest", true);
			FinishLatentTask(*OwnerCompRef, EBTNodeResult::Succeeded);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PatrolFailed"));
			BlackboardComp->SetValueAsBool("IsRest", true);
			FinishLatentTask(*OwnerCompRef, EBTNodeResult::Failed);
		}
	}
}

// void UBTT_Patrol::OnMoveCompleted(FAIRequestID RequestID, const EPathFollowingResult& Result)
// {
// 	if (Result.Code == EPathFollowingResult::Success)
// 	{
// 		UE_LOG(LogTemp, Log, TEXT("Patrol point reached successfully"));
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Failed to reach patrol point"));
// 	}
//
// 	if (OwnerCompRef)
// 	{
// 		FinishLatentTask(*OwnerCompRef, Result.Code == EPathFollowingResult::Success ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
// 	}
//
// 	// Delegate 정리
// 	ACAIController* BaseAIController = Cast<ACAIController>(OwnerCompRef->GetAIOwner());
// 	if (BaseAIController)
// 	{
// 		BaseAIController->ReceiveMoveCompleted.RemoveDynamic(this, &UBTT_Patrol::OnMoveCompleted);
// 	}
// }
