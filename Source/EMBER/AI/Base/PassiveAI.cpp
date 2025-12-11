#include "AI/Base/PassiveAI.h"

#include "CAIController.h"
#include "C_StateComponent.h"
#include "AI/AnimInstance/BaseAIAnimInstance.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Boss/Griffon.h"

APassiveAI::APassiveAI()
{
	ClosestActor = nullptr;
	EnemyActors = TArray<AActor*>();
	ClosestDistanceBoundary = 1000.0f;
	ClosestDistance = FLT_MAX;
}

void APassiveAI::BeginPlay()
{
	ABaseAI::BeginPlay();
}

float APassiveAI::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                             class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ACAIController* AIController = Cast<ACAIController>(GetController()))
	{
		if (TObjectPtr<UBlackboardComponent> BlackboardComponent = Cast<UBlackboardComponent>(
	AIController->GetBlackboardComponent()))
		{
			BlackboardComponent->SetValueAsObject("TargetActor", DamageCauser->GetOwner());
		}
	}
	
	if (TObjectPtr<UCBehaviorTreeComponent> BTState = Cast<UCBehaviorTreeComponent>(
		this->GetComponentByClass(UCBehaviorTreeComponent::StaticClass())))
	{
		BTState->SetRunMode();
	}

	return ActualDamage;
}

void APassiveAI::OnDeath()
{
	Super::OnDeath();
}
