// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTT_GriffonFly.h"

#include "CAIController.h"
#include "Base/BaseAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTT_GriffonFly::UBTT_GriffonFly()
{
	NodeName = "Fly";
	bNotifyTick = true;
	FlyHeight = 700.0f;
}

EBTNodeResult::Type UBTT_GriffonFly::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIController = Cast<ACAIController>(OwnerComp.GetAIOwner());
	BlackboardComp = OwnerComp.GetBlackboardComponent();
	AI = Cast<ABaseAI>(AIController->GetPawn());

	AIController->StopMovement(); // 이동 정지
	BlackboardComp->SetValueAsFloat("OriginHeight", AI->GetActorLocation().Z);
	AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));
	if (Target == nullptr)
	{
		//UE_LOG(LogTemp, Error, TEXT("GuardTarget is null"));
		return EBTNodeResult::Failed;
	}

	AI->GetCharacterMovement()->StopMovementImmediately();
	AI->GetCharacterMovement()->GravityScale = 0.0f;
	AI->GetCharacterMovement()->Velocity = FVector(0, 0, 500);
	AI->GetCharacterMovement()->SetMovementMode(MOVE_Flying);

	return EBTNodeResult::InProgress;
}

void UBTT_GriffonFly::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	float OriginHeight = BlackboardComp->GetValueAsFloat("OriginHeight");

	if (AI->GetActorLocation().Z > OriginHeight + FlyHeight)
	{
		AI->GetCharacterMovement()->Velocity.Z = 0.0f;

		uint8 StateValue = BlackboardComp->GetValueAsEnum("AIState");

		if (StateValue != static_cast<uint8>(EAnimalState::Detect))
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}
