#include "AI/Task/BTT_Speed.h"

#include "AIController.h"
#include "C_CharacterMovementComponent.h"
#include "AI/Base/HumanAIBase.h"

UBTT_Speed::UBTT_Speed()
{
	NodeName = "Speed";
}

EBTNodeResult::Type UBTT_Speed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	move.Get()->SetSpeed(SpeedType);
	return EBTNodeResult::Succeeded;
}
