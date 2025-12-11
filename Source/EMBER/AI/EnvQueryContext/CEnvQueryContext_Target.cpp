// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnvQueryContext/CEnvQueryContext_Target.h"

#include "CAIController.h"
#include "Base/BaseAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EQSTestingPawn.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "GameFramework/PlayerStart.h"
#include "kismet/GameplayStatics.h"

void UCEnvQueryContext_Target::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	AEQSTestingPawn* eqsPawn = Cast<AEQSTestingPawn>(QueryInstance.Owner.Get());
	if(eqsPawn != nullptr)
	{
		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsOfClass(eqsPawn->GetWorld(), APlayerStart::StaticClass(), actors);

		if(actors.Num() > 0)
			UEnvQueryItemType_Actor::SetContextHelper(ContextData, actors[0]);

		return;
	}

	ABaseAI* ai = Cast<ABaseAI>(QueryInstance.Owner.Get());
	if(ai == nullptr)
	{
		UE_LOG(LogTemp, Error, L"eqs target, ai is null");
		return;
	}
	ACAIController* controller = Cast<ACAIController>(ai->GetController());
	if (controller == nullptr)
	{
		UE_LOG(LogTemp, Error, L"eqs target, controller is null");
		return;
	}
	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
	if (blackboard == nullptr)
	{
		UE_LOG(LogTemp, Error, L"eqs target, blackboard is null");
		return;
	}
	AActor* target = Cast<AActor>(blackboard->GetValueAsObject("TargetActor"));
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, target);
}
