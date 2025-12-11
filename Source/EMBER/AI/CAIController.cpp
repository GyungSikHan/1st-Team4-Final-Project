// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CAIController.h"
#include "AI/Base/HumanAIBase.h"
#include "Component/C_StateComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/CBehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ACAIController::ACAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
	
	Sight->SightRadius = 600.0f;
	Sight->LoseSightRadius = 800.f;
	Sight->PeripheralVisionAngleDegrees = 45;
	Sight->SetMaxAge(2);
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
	//GetSenseImplementation 자료형 타입을 넘겨주는 함수
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AI = Cast<ABaseAI>(InPawn);

	if (AI->GetBehaviorTree() == nullptr)
	{
		UE_LOG(LogTemp, Error, L"Behavior is null");
		return;
	}

	AIState = Cast<UC_StateComponent>(AI->GetComponentByClass(UC_StateComponent::StaticClass()));
	UBlackboardComponent* bb = GetBlackboardComponent();
	UseBlackboard(AI->GetBehaviorTree()->GetBlackboardAsset(), bb);

	Behavior = Cast<UCBehaviorTreeComponent>(AI->GetComponentByClass(UCBehaviorTreeComponent::StaticClass()));
	Behavior->SetBlackboard(Blackboard);

	RunBehaviorTree(AI->GetBehaviorTree());
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	Perception->OnTargetPerceptionUpdated.AddDynamic(this, &ACAIController::OnTargetPerceptionUpdated);
}

void ACAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	Perception->GetCurrentlyPerceivedActors(nullptr, Actors);

	if (Stimulus.WasSuccessfullySensed() && Actor->Tags.Contains(FName("Player")))
	{
		if (Behavior)
		{
			Blackboard->SetValueAsObject("TargetActor", Actor);
			AIState->SetDetectMode();
			UE_LOG(LogTemp, Warning, TEXT("Detected target"));
		}
	}
	else
	{
		Actors.RemoveSingle(Actor);
		UE_LOG(LogTemp, Warning, TEXT("Actors.Num(): %d"), Actors.Num());
		if (Actors.Num() == 0 && !Behavior->IsActionMode() && !Behavior->IsRunMode() && !Behavior->IsHittedMode())
		{
			UE_LOG(LogTemp, Warning, TEXT("Perception: Target Lost"));
			AIState->SetIdleMode();
			Blackboard->SetValueAsObject("TargetActor", nullptr);
			return;
		}
	}
}
