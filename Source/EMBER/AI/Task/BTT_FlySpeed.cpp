#include "AI/Task/BTT_FlySpeed.h"
#include "C_CharacterMovementComponent.h"
#include "AI/Base/BaseAI.h"
#include "AI/CAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTT_FlySpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	TObjectPtr<AAIController>controller = Cast<AAIController>(OwnerComp.GetOwner());
	if(controller == nullptr)
	{
		UE_LOG(LogTemp, Error, L"controller is null");
		return EBTNodeResult::Failed;
	}
	TObjectPtr<ABaseAI> ai = Cast<ABaseAI>(controller->GetPawn());
	if(ai == nullptr)
	{
		UE_LOG(LogTemp, Error, L"ai is null");
		return EBTNodeResult::Failed;
	}
	TObjectPtr<UC_CharacterMovementComponent> move = Cast<UC_CharacterMovementComponent>(ai->GetComponentByClass(UC_CharacterMovementComponent::StaticClass()));
	if(move == nullptr)
	{
		UE_LOG(LogTemp, Error, L"move component is null");
		return EBTNodeResult::Failed;
	}
	// UE_LOG(LogTemp, Error, L"%s, %p", *ai->GetActorLabel(), this);

	UBlackboardComponent* BlackboardComponent = Cast<UBlackboardComponent>(OwnerComp.GetBlackboardComponent());
	move.Get()->SetFlySpeed(FlySpeedType);
	return EBTNodeResult::Succeeded;
}
