// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/CBTDecorator_FarAttack.h"

#include "CAIController.h"
#include "Base/BaseAI.h"
#include "Kismet/KismetMathLibrary.h"
#if WITH_EDITOR
FName UCBTDecorator_FarAttack::GetNodeIconName() const
{
	return "Far Attack Range";
}
#endif

bool UCBTDecorator_FarAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	TObjectPtr<ACAIController> controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
	{
		UE_LOG(LogTemp, Error, L"AttackRange Service, controller is null");
		return false;
	}
	TObjectPtr<ABaseAI> ai = Cast<ABaseAI>(controller->GetPawn());
	if (ai == nullptr)
	{
		UE_LOG(LogTemp, Error, L"AttackRange Service, ai is null");
		return false;
	}

	if (bDebug == true)
	{
		FVector start = ai->GetActorLocation();
		FVector end = start;
		DrawDebugCylinder(ai->GetWorld(),start,end,FarAttackRange,10,FColor::Black);
	}

	TObjectPtr<UCBehaviorTreeComponent> behavior = Cast<UCBehaviorTreeComponent>(ai->GetComponentByClass(UCBehaviorTreeComponent::StaticClass()));
	if (behavior == nullptr)
	{
		UE_LOG(LogTemp, Error, L"AttackRange Service, behavior is null");
		return false;
	}

	TObjectPtr<ACharacter> target = behavior->GetTarget();
	if (target == nullptr)
	{
		UE_LOG(LogTemp, Error, L"AttackRange Service, Target is null");
		return false;
	}

	float distance = ai->GetDistanceTo(target);

	if (distance > FarAttackRange)
		return false;

	return true;
}
