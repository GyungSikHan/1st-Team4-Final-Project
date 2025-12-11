#include "BTT_RandomPatrol.h"
#include "CAIController.h"
#include "AI/Base/HumanAIBase.h"
#include "NavigationSystem.h"
#include "BehaviorTree/CBehaviorTreeComponent.h"
#include "Navigation/PathFollowingComponent.h"


UBTT_RandomPatrol::UBTT_RandomPatrol()
{
	NodeName = " Random Patrol";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_RandomPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	TObjectPtr<ACAIController> controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if(controller == nullptr)
	{
		UE_LOG(LogTemp, Warning, L"controller is null");
		return EBTNodeResult::Failed;
	}
	TObjectPtr<ABaseAI> ai = Cast<ABaseAI>(controller->GetPawn());
	if (ai == nullptr)
	{
		UE_LOG(LogTemp, Error, L"ai is null");
		return EBTNodeResult::Failed;
	}
	TObjectPtr<UCBehaviorTreeComponent> behavior = Cast<UCBehaviorTreeComponent>(ai->GetComponentByClass(UBehaviorTreeComponent::StaticClass()));
	if (behavior == nullptr)
	{
		UE_LOG(LogTemp, Error, L"behavior is null");
		return EBTNodeResult::Failed;
	}

	FVector location = ai->GetActorLocation();
	UNavigationSystemV1* navi = FNavigationSystem::GetCurrent<UNavigationSystemV1>(ai->GetWorld());

	if(navi == nullptr)
	{
		UE_LOG(LogTemp, Error, L"navigation system is null");
		return EBTNodeResult::Failed;
	}

	FNavLocation point(location);
	while (true)
	{
		if(navi->GetRandomPointInNavigableRadius(location, RandomRaduis, point))
			break;
	}

	behavior->SetPatrolLocation(point.Location);
	if (bDebugMode == true)
		DrawDebugSphere(ai->GetWorld(), point, 25, 25, FColor::Green, true, 5);
	return EBTNodeResult::InProgress;
}

void UBTT_RandomPatrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	TObjectPtr<ACAIController> controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
	{
		UE_LOG(LogTemp, Warning, L"controller is null");
		return;
	}
	TObjectPtr<ABaseAI> ai = Cast<ABaseAI>(controller->GetPawn());
	if (ai == nullptr)
	{
		UE_LOG(LogTemp, Error, L"ai is null");
		return;
	}
	TObjectPtr<UCBehaviorTreeComponent> behavior = Cast<UCBehaviorTreeComponent>(ai->GetComponentByClass(UBehaviorTreeComponent::StaticClass()));
	if (behavior == nullptr)
	{
		UE_LOG(LogTemp, Error, L"behavior is null");
		return;
	}

	FVector location = behavior->GetPatrolLocation();
	EPathFollowingRequestResult::Type result = controller->MoveToLocation(location, AcceptanceDistance,false);
	switch (result)
	{
	case EPathFollowingRequestResult::Type::Failed:
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		break;
	case EPathFollowingRequestResult::Type::AlreadyAtGoal:
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		break;
	}
}
