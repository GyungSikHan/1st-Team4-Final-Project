#include "AI/Task/BTT_RotateToActor.h"
#include "AIController.h"
#include "Base/BaseAI.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_RotateToActor::UBTT_RotateToActor()
{
	NodeName = TEXT("Turn To Target");
	bNotifyTick = true; // TickTask() 사용을 활성화
}

EBTNodeResult::Type UBTT_RotateToActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;
	AI = Cast<ABaseAI>(AIController->GetPawn());
	BlackboardComp = OwnerComp.GetBlackboardComponent();
	Target = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
	if (!Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetActor is nullptr!"));
		return EBTNodeResult::Failed;
	}
	FVector Direction = (Target->GetActorLocation() - AI->GetActorLocation()).GetSafeNormal();
	TargetRotation = Direction.Rotation();

	FVector Forward = AI->GetActorForwardVector();
	FVector ToTarget = (Target->GetActorLocation() - AI->GetActorLocation()).GetSafeNormal2D();
	float RightDot = FVector::CrossProduct(Forward, ToTarget).Z;
	float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Forward, ToTarget)));
	if (Angle < 15.0f)
	{
		return EBTNodeResult::InProgress;
	}
	if (RightDot > 0.0f && TurnRightMontage) // 오른쪽에 있음
	{
		AI->PlayAnimMontage(TurnRightMontage);
	}
	else if (RightDot < 0.0f && TurnLeftMontage) // 왼쪽에 있음
	{
		AI->PlayAnimMontage(TurnLeftMontage);
	}
	return EBTNodeResult::InProgress; // TickTask()에서 계속 업데이트됨
}

void UBTT_RotateToActor::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	CurrentRotation = AI->GetActorRotation();
	float RotationSpeed = 3.2f;

	// 목표 방향으로 부드럽게 회전
	FRotator InterpolatedRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, RotationSpeed);
	AI->SetActorRotation(InterpolatedRotation);

	// 일정 각도 이하로 가까워지면 회전 종료
	if (FMath::Abs(CurrentRotation.Yaw - TargetRotation.Yaw) < 1.0f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
