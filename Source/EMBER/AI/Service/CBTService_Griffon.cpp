// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/CBTService_Griffon.h"
#include "AI/CAIController.h"
#include "AI/Boss/Griffon.h"
#include "Component/C_StateComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UCBTService_Griffon::UCBTService_Griffon()
{
	Interval = 1.0f;
	RandomDeviation = 0.0f;
}

void UCBTService_Griffon::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	TObjectPtr<ACAIController> Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if(Controller.Get() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Controller is null");
		return;
	}
	TObjectPtr<AGriffon> AI = Cast<AGriffon>(Controller->GetPawn());
	if (AI.Get() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"AI is null");
		return;
	}
	TObjectPtr<UC_StateComponent> State = Cast<UC_StateComponent>(AI->GetComponentByClass(UC_StateComponent::StaticClass()));
	if (State.Get() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"state is null");
		return;
	}
	TObjectPtr<UCBehaviorTreeComponent> BTState = Cast<UCBehaviorTreeComponent>(AI->GetComponentByClass(UCBehaviorTreeComponent::StaticClass()));
	if (BTState.Get() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"BTState is null");
		return;
	}
	UBlackboardComponent* Blackboard = Cast<UBlackboardComponent>(OwnerComp.GetBlackboardComponent());
	
	if (State->IsDetectMode())
	{
		BTState->SetDetectMode();
	}
	if (State->IsHittdMode())
	{
		BTState->SetHittedMode();
	}
	if (State->IsDeadMode())
	{
		BTState->SetDeadMode();
	}
	if (State->IsIdleMode())
	{
		BTState->SetIdleMode();
	}
	
	//Target이 없으면 Patrol
	TObjectPtr<ACharacter> Target = BTState->GetTarget(); 
	if(Target == nullptr)
	{
		State->SetIdleMode();
		BTState->SetIdleMode();
		return;
	}
	//Target과 가까우면 AttackMode
	float distance = AI.Get()->GetDistanceTo(Target);
	if(distance < ActionRange)
	{
		State->SetCombatMode();
		BTState->SetCombatMode();
		return;
	}
	FVector OriginLocation = Controller->GetBlackboardComponent()->GetValueAsVector("OriginLocation");
	FVector TargetLocation = Target->GetActorLocation();
	float Distance = FVector::Dist(OriginLocation, TargetLocation);
	// if (Distance >= 3000.0f)
	// {
	// 	UE_LOG(LogTemp, Error, L"Distance is too Far in Service");
	// 	BTState->SetIdleMode();
	// 	return;
	// }
}